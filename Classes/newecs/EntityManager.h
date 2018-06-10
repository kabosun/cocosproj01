#pragma once

#include <vector>
#include <deque>
#include <list>
#include <functional>
#include <unordered_map>
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
		std::unordered_map<std::type_index, int> m_SystemIndex;
		
	public:
		virtual ~EntityManager();
		
		void Init()
		{
			m_allocator.Init();
		}
		
		/**
		 * エンティティを生成します。
		 */
		template<class T>
		void CreateArchetype()
		{
			printf("create archetype %s\n", typeid(T).name());
		}
		
		template<class T, class S, class... Types>
		void CreateArchetype()
		{
			CreateArchetype<T>();
			CreateArchetype<S, Types...>();
		}
		
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
		
		template<class T, typename... Args>
		void RegisterSystem(Args&&... args)
		{
			System* system = new T(args...);
			m_Systems.push_back(system);
			m_SystemIndex[typeid(T)] = static_cast<int>(m_Systems.size());
		}
		
		void Update(float delta);
	};
	
}
