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

	static const int MINIMUM_FREE_INDICES = 1024;
	
	class EntityManager
	{
		std::vector<char> m_Generation;
		std::deque<int> m_Free;
		std::vector<System*> m_Systems;
		ComponentAllocator m_allocator;
		std::unordered_map<std::type_index, int> m_SystemIndex;
		
		std::vector<Entity> m_removelist;

	public:
		virtual ~EntityManager();
		
		void Init()
		{
			m_allocator.Init();
		}
		
		template<typename... Args>
		Archetype CreateArchetype(Args... args)
		{
			std::vector<ComponentInfo> list = {args...};
			
			Archetype a;
			for (ComponentInfo& info : list)
			{
				a.set(info.Index);
			}
			
			// chunkの生成
			m_allocator.allocatechunk(a, list);
			
			return a;
		}

		template<typename... Args>
		Filter CreateFilter(Args... args)
		{
			std::vector<ComponentInfo> list = { args... };

			Filter a;
			for (ComponentInfo& info : list)
			{
				a.set(info.Index);
			}

			return a;
		}
		
		Entity CreateEntity();
		Entity CreateEntity(Archetype archetype);
		
		template<class T, typename... Args>
		void AssignComponent(Entity entity, Args&&... args)
		{
			m_allocator.assign<T>(entity, args...);
		}
		
		template<class T>
		T* GetComponent(Entity entity) const
		{
			T* components = m_allocator.gethead<T>();
			
			int index = m_allocator.gethandle(entity);
			return &components[index];
		}
		
		ComponentGroup GetComponentGroup(const Archetype& archetype) const
		{
			return m_allocator.getcomponentgroup(archetype);
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
			
			InitSystem(system);
		}
		
		void Update(float delta);

		void GC();
		
	private:
		void InitSystem(System * system);
	};
	
}
