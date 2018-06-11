#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>
#include "Entity.h"

#ifdef _MSC_VER
#include "cocos2d.h"
#define log(...)		cocos2d::log(__VA_ARGS__)

#else
#define log(...)
#endif

namespace ecs
{
	// すべてのコンポーネントを同じサイズで確保していく
	class ComponentAllocator
	{
		static const int m_defaultSpaceSize = 128;
		
		struct Chunk
		{
			static const int chunksize = 64 * 1024;
			
			Archetype archetype;	// 定義できるコンポーネント数が最大256

			int maxEntityCount = 0;	// エンティティの最大数
			int componentCount = 0;	// コンポーネントの数
			int length = 0;
			int maxlength = 0;	// entityに割り当てられるcomponentの最大数
			int useSize = 0;

			void* headLut[256] = {};
			int sizeLut[256] = { 0 };

			void* head = nullptr;
			void* buffer = nullptr;
			Entity* entities = nullptr;	// buffer内に確保される
		};
		
		int m_maxSize = 0;
		int m_usesize = 0;
		void* m_buffer = nullptr;
		void* m_head = nullptr;
		int m_current = 0;

		int m_totalchunkcount = 0;
		int m_usechunkcount = 0;
		Chunk* m_chunkman = nullptr;	// chunkの管理領域

		int m_componentIndex = 0;
		std::unordered_map<std::type_index, int> m_componentIndexLut;

		std::unordered_map<std::type_index, Chunk> m_componentLut;
		std::unordered_map<EntityId, int> m_entityLut;	// entityがどのchunkの何番目か. entityが移動するときにも更新しないといけない
		
	public:
		
		void Init(int maxSize = 4 * 1024 * 1024/* 4M */)
		{
			assert(m_buffer == nullptr);
			m_buffer = operator new (maxSize);
			m_head = m_buffer;
			m_maxSize = maxSize;

			// max chunk
			m_totalchunkcount = maxSize / (Chunk::chunksize + sizeof(Chunk));

			// 管理領域の割りあてと初期化
			m_chunkman = static_cast<Chunk*>(m_head);
			int chunkmansize = m_totalchunkcount * sizeof(Chunk);
			for (int i = 0; i < m_totalchunkcount; i++)
			{
				new(&m_chunkman[i])Chunk;
			}

			m_head = static_cast<char*>(m_head) + chunkmansize;
			m_usesize += chunkmansize;

			log("chunk (size:%d) count:%d", sizeof(Chunk), m_totalchunkcount);
		}
		
		void destroy()
		{
			if (m_buffer)
			{
				operator delete (m_buffer);
			}
		}
		
		virtual ~ComponentAllocator()
		{
			destroy();
		}
		
		int gethandle(Entity entity) const
		{
			assert(m_entityLut.count(entity.Id) != 0);
			return m_entityLut.at(entity.Id);
		}
		
		template<class T>
		T* gethead(int chunkindex=0) const
		{
			assert(chunkindex >= 0 && m_totalchunkcount > chunkindex);
			assert(m_componentIndexLut.count(typeid(T)) > 0);

			const Chunk* chunk = &m_chunkman[chunkindex];

			int index = m_componentIndexLut.at(typeid(T));
			return static_cast<T*>(chunk->headLut[index]);
		}

		int length() const
		{
			int chunkindex = 0;
			const Chunk* chunk = &m_chunkman[chunkindex];
			return chunk->length;
		}
		
		template<class T, typename... Args>
		void alloc(Entity entity, Args&&... args)
		{
			// chunkがなければ空きchunkにメモリを割り当てる
			Chunk* chunk = get_or_alloc_chunk();

			// componentに番号をふる
			int componentHandle = getcomponenthandle<T>();

			// componentがなければ割り当てる
			void* p = get_or_alloc_compnent(chunk, componentHandle, sizeof(T));
			T* components = static_cast<T*>(p);

			if (m_entityLut.count(entity.Id) == 0)
			{
				m_entityLut[entity.Id] = chunk->length;
				chunk->entities[chunk->length] = entity;
				chunk->length++;
			}

			int index = m_entityLut.at(entity.Id);
			new(&components[index])T(args...);
		}
		
		void free(Entity entity)
		{
			assert(m_entityLut.count(entity.Id) > 0);
			
			int chunkindex = 0;
			Chunk* chunk = &m_chunkman[chunkindex];

			int index = m_entityLut.at(entity.Id);
			
			// entityのindexを再配置する
			// TODO 再配置コストが高い
			for (int i=0; i<256; i++)
			{
				void* buffer = chunk->headLut[i];
				if (buffer == nullptr) continue;

				int size = chunk->sizeLut[i];
				void* src = static_cast<char*>(buffer) + (size * (chunk->length - 1));
				void* dst = static_cast<char*>(buffer) + (size * index);
				memcpy(dst, src, size);
			}

			Entity lastEntity = chunk->entities[index] = chunk->entities[chunk->length - 1];
			chunk->length--;

			m_entityLut[lastEntity.Id] = index;
			m_entityLut.erase(entity.Id);
		}

	private:

		Chunk* get_or_alloc_chunk()
		{
			if (m_usechunkcount == 0)
			{
				Chunk* chunk = &m_chunkman[m_usechunkcount];
				chunk->buffer = m_head;
				chunk->head = m_head;
				m_head = static_cast<char*>(m_head) + Chunk::chunksize;
				m_usesize += Chunk::chunksize;
				m_usechunkcount++;
			}

			return &m_chunkman[0];	// とりあえずchunkは1つ
		}

		void* get_or_alloc_compnent(Chunk* chunk, int componentHandle, int componentsize)
		{
			if (chunk->headLut[componentHandle] == nullptr)
			{
				int freespace = Chunk::chunksize - chunk->useSize;

				if (chunk->maxlength == 0)
				{
					chunk->maxlength = std::min(128, freespace / componentsize);	// まだ最大128に制限しておく
					assert(chunk->maxlength > 0);
				}

				assert(componentsize * chunk->length <= freespace);	// 空き不足

				// entity領域を確保する
				chunk->entities = static_cast<Entity*>(chunk->head);
				chunk->head = static_cast<char*>(chunk->head) + (sizeof(Entity) * chunk->maxlength);

				// chunkのheadからメモリを確保
				chunk->headLut[componentHandle] = chunk->head;
				chunk->sizeLut[componentHandle] = componentsize;
				chunk->head = static_cast<char*>(chunk->head) + (componentsize * chunk->maxlength);
				chunk->useSize += (componentsize * chunk->maxlength);
			}
			return chunk->headLut[componentHandle];
		}

		template<class T>
		int getcomponenthandle()
		{
			int componentHandle = 0;
			if (m_componentIndexLut.count(typeid(T)) == 0)
			{
				componentHandle = m_componentIndex;
				m_componentIndexLut[typeid(T)] = componentHandle;
				m_componentIndex++;
			}
			else
			{
				componentHandle = m_componentIndexLut.at(typeid(T));
			}
			return componentHandle;
		}
	};
}
