#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>
#include "Entity.h"

#ifdef _MSC_VER
#include "cocos2d.h"
#define log(...)		cocos2d::log(__VA_ARGS__)
#else
#define log(x, ...)		printf(x "\n", __VA_ARGS__)
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
			
			int add(Entity entity)
			{
				assert(length < maxlength);
				
				entities[length] = entity;
				length++;
				return length - 1;
			}
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

		std::unordered_map<std::type_index, Chunk> m_componentLut;
		std::unordered_map<EntityId, int> m_entityLut;	// entityがどのchunkの何番目か. entityが移動するときにも更新しないといけない
		
		std::unordered_map<Archetype, int> m_ArchetypeChunkLut;
		
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

			log("chunk (size:%zu) count:%d", sizeof(Chunk), m_totalchunkcount);
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

			const Chunk* chunk = &m_chunkman[chunkindex];

			int index = T::Info().Index;
			return static_cast<T*>(chunk->headLut[index]);
		}
		
		Entity* getentity(int chunkindex=0) const
		{
			assert(chunkindex >= 0 && m_totalchunkcount > chunkindex);
			
			const Chunk* chunk = &m_chunkman[chunkindex];
			return chunk->entities;
		}

		int length() const
		{
			int chunkindex = 0;
			const Chunk* chunk = &m_chunkman[chunkindex];
			return chunk->length;
		}
		
		void allocatechunk(Archetype archetype, std::vector<ComponentInfo> list)
		{
			// chunkがなければ空きchunkにメモリを割り当てる
			Chunk* chunk = &m_chunkman[get_or_alloc_chunk(archetype)];
			
			for (auto info : list)
			{
				get_or_alloc_compnent(chunk, info.Index, info.Size);
			}
		}
		
		void allocateentity(Entity entity, Archetype archetype)
		{
			int chunkindex = m_ArchetypeChunkLut.at(archetype);
			Chunk* chunk = &m_chunkman[chunkindex];
			
			int index = chunk->add(entity);
			
			m_entityLut[entity.Id] = (chunkindex << 16) | index;
		}
		
		template<class T, typename... Args>
		void assign(Entity entity, Args&&... args)
		{
			int index = m_entityLut.at(entity.Id);
			int chunkindex = index >> 16;
			int entityindex = index & 65535;
			
			Chunk* chunk = &m_chunkman[chunkindex];
			
			void* p = getcomponent(chunk, T::Info().Index);
			T* components = static_cast<T*>(p);
			
			new(&components[entityindex])T(args...);
		}
		
		template<class T, typename... Args>
		void alloc(Entity entity, Args&&... args)
		{
			// chunkがなければ空きchunkにメモリを割り当てる
			Chunk* chunk = &m_chunkman[0];

			// componentに番号をふる
			int componentHandle = T::Info().Index;

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

		int get_or_alloc_chunk(Archetype archetype)
		{
			if (m_ArchetypeChunkLut.count(archetype) > 0)
			{
				return m_ArchetypeChunkLut.at(archetype);
			}
			
			Chunk* chunk = &m_chunkman[m_usechunkcount];
			chunk->buffer = m_head;
			chunk->head = m_head;
			m_head = static_cast<char*>(m_head) + Chunk::chunksize;
			m_usesize += Chunk::chunksize;
			
			int index = m_usechunkcount;
			m_ArchetypeChunkLut[archetype] = index;
			m_usechunkcount++;

			return index;
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
		
		void* getcomponent(Chunk* chunk, int index) const
		{
			return chunk->headLut[index];
		}
	};
}
