#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>
#include "Entity.h"

namespace ecs
{
	// すべてのコンポーネントを同じサイズで確保していく
	class ComponentAllocator
	{
		static const int m_defaultSpaceSize = 128;
		
		struct Chunk
		{
			int size = 0;	// コンポーネントのサイズ
			void* buffer = nullptr;
		};
		
		int m_maxSize = 0;
		int m_use_size = 0;
		void* m_buffer = nullptr;
		void* m_head = nullptr;
		int m_current = 0;
		
		std::unordered_map<std::type_index, Chunk> m_componentLut;
		std::unordered_map<EntityId, int> m_entityLut;
		std::vector<Entity> m_indexLut;
		
	public:
		
		void Init(int maxSize = 64 * 1024/* 64k */)
		{
			assert(m_buffer == nullptr);
			m_buffer = operator new (maxSize);
			m_head = m_buffer;
			m_maxSize = maxSize;
			
			m_indexLut.resize(m_defaultSpaceSize);
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
		
		int length() const
		{
			return m_current;
		}
		
		int gethandle(Entity entity) const
		{
			assert(m_entityLut.count(entity.Id) != 0);
			return m_entityLut.at(entity.Id);
		}
		
		template<class T>
		T* getchunk() const
		{
			assert(m_componentLut.count(typeid(T)) != 0);
			
			const Chunk& chunk = m_componentLut.at(typeid(T));
			T* head = static_cast<T*>(chunk.buffer);
			
			return head;
		}
		
		template<class T, typename... Args>
		void alloc(Entity entity, Args&&... args)
		{
			if (m_componentLut.count(typeid(T)) == 0)
			{
				// 初回アサイン時はchunkを生成する
				int size = m_defaultSpaceSize;
				m_use_size += sizeof(T) * size;
				assert(m_maxSize > m_use_size);
				
				T* buf = static_cast<T*>(m_head);
				
				T* p = static_cast<T*>(m_head);
				p += size;
				m_head = static_cast<void*>(p);
				
				Chunk chunk;
				chunk.size = sizeof(T);
				chunk.buffer = buf;
				m_componentLut[typeid(T)] = chunk;
			}
			
			// 初期化
			const Chunk& chunk = m_componentLut.at(typeid(T));
			T* buf = static_cast<T*>(chunk.buffer);
			
			new(&buf[m_current])T(args...);
			
			// 未登録のentityならカーソルを進める
			if (m_entityLut.count(entity.Id) == 0)
			{
				m_indexLut[m_current] = entity;
				m_entityLut[entity.Id] = m_current;
				m_current++;
			}
		}
		
		void free(Entity entity)
		{
			assert(m_entityLut.count(entity.Id) > 0);
			
			int index = m_entityLut[entity.Id];
			
			// entityのindexを再配置する
			for (auto pair : m_componentLut)
			{
				Chunk& chunk = pair.second;
				void* src = static_cast<char*>(chunk.buffer) + (chunk.size * (m_current - 1));
				void* dst = static_cast<char*>(chunk.buffer) + (chunk.size * index);
				memcpy(dst, src, chunk.size);
			}
			
			Entity lastEntity = m_indexLut[m_current - 1];
			m_entityLut[lastEntity.Id] = index;
			m_entityLut.erase(entity.Id);
			m_current--;
		}
	};
}
