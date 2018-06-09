#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeindex>
#include "Component.h"

namespace ecs2
{
	class ComponentRegistry
	{
		EntityRegistry* m_EntityRegistry;
		std::map<std::type_index, std::shared_ptr<IComponent>> m_ComponentSystems;
		std::vector<std::shared_ptr<IUpdatable>> m_UpdatableSystems;
		std::vector<std::shared_ptr<IUpdatable>> m_PostUpdatableSystems;
		
	public:
		
		virtual ~ComponentRegistry()
		{}

		void Initialize(EntityRegistry* registry)
		{
			m_EntityRegistry = registry;
		}
		
		template<typename T>
		std::shared_ptr<T> AddComponent()
		{
			auto p = std::make_shared<T>();
			p->m_ComponentRegistry = this;
			m_ComponentSystems[typeid(T)] = p;
			
			return p;
		}
		
		template<typename T>
		std::shared_ptr<T> AddUpdatableComponent()
		{
			auto p = std::make_shared<T>();
			p->m_ComponentRegistry = this;
			m_ComponentSystems[typeid(T)] = p;
			m_UpdatableSystems.push_back(p);
			
			return p;
		}
		
		template<typename T>
		std::shared_ptr<T> AddPostUpdatableComponent()
		{
			auto p = std::make_shared<T>();
			p->m_ComponentRegistry = this;
			m_ComponentSystems[typeid(T)] = p;
			m_PostUpdatableSystems.push_back(p);
			
			return p;
		}
		
		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			return std::static_pointer_cast<T>(m_ComponentSystems[typeid(T)]);
		}

		ComponentHandle AttachComponent(Entity entity, std::type_index type)
		{
			return m_ComponentSystems[type]->Attach(entity);
		}
		
		void Update(float dt)
		{
			for (auto pair : m_ComponentSystems)
			{
				pair.second->Assign();
			}
			
			for (auto&& system : m_UpdatableSystems)
			{
				system->Update(*m_EntityRegistry, dt);
			}
		}
		
		void PostUpdate(float dt)
		{
			for (auto&& system : m_PostUpdatableSystems)
			{
				system->Update(*m_EntityRegistry, dt);
			}
		}
		
		void GC()
		{
			for (auto pair : m_ComponentSystems)
			{
				pair.second->GC(*m_EntityRegistry);
			}
		}
		
		Entity CreateEntity(const Archetype archetype)
		{
			Entity entity = m_EntityRegistry->Create();

			for (auto type : archetype)
				AttachComponent(entity, type);

			return entity;
		}
		
		bool AliveEntity(Entity entity) const
		{
			return m_EntityRegistry->Alive(entity);
		}
		
		void RemoveEntity(Entity entity)
		{
			return m_EntityRegistry->Remove(entity);
		}
		void AddEntityEventListener(IEntityEventListener* listener)
		{
			m_EntityRegistry->AddEventListener(listener);
		}
		
		void RemoveEntityEventListener(IEntityEventListener* listener)
		{
			m_EntityRegistry->RemoveListener(listener);
		}
	};
	
}
