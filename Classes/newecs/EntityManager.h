#pragma once

#include <vector>
#include <deque>
#include <list>
#include <functional>
#include "Entity.h"
#include "ComponentAllocator.h"

namespace ecs
{
	class System;
	
	using Archetype = int;
	
	static const int MINIMUM_FREE_INDICES = 1024;
	
	class EntityManager
	{
		std::vector<char> m_Generation;
		std::deque<int> m_Free;
		std::vector<System*> m_Systems;
		ComponentAllocator m_allocator;
		
	public:
		
#if 0
		template<class... T>
		void CreateArchetype()
		{
			void* data = nullptr;
			AllocateComponent<T...>(data);
		}
		
		template<class T>
		void AllocateComponent(void* data)
		{
			//printf("types:%d\n", sizeof...(types));
			printf("create archetype %s\n", typeid(T).name());
		}
		
		template<class T, class S, class... R>
		void AllocateComponent(void* data)
		{
			AllocateComponent<T>(data);
			AllocateComponent<S, R...>(data);
		}
#endif
		void Init()
		{
			m_allocator.Init();
		}
		
		/**
		 * エンティティを生成します。
		 */
		Entity CreateEntity();
		Entity CreateEntity(Archetype archetype);
		
		template<class T, typename... Args>
		void AssignComponent(Entity entity, Args&&... args)
		{
			m_allocator.alloc<T>(entity, args...);
		}
		
		template<class T>
		T* GetComponent(Entity entity) const
		{
			T* components = m_allocator.getchunk<T>();
			
			int index = m_allocator.gethandle(entity);
			return &components[index];
		}
		
		template<class T>
		T* GetComponentHead() const
		{
			T* components = m_allocator.getchunk<T>();
			
			return components;
		}
		
		int length() const
		{
			return m_allocator.length();
		}
		
		/**
		 * 指定されたエンティティが生存しているか判定します。
		 */
		bool AliveEntity(Entity entity) const;
		
		/**
		 * エンティティを削除します。
		 */
		void RemoveEntity(Entity entity);
		
		void RegisterSystem(System* system);
		
		void Update(float delta);
	};
	
}
