#include "EntityManager.h"
#include "System.h"

namespace ecs
{
	EntityManager::~EntityManager()
	{
		for (System* system : m_Systems)
		{
			delete system;
		}
	}
	
	Entity EntityManager::CreateEntity()
	{
		int index = 0;
		
		if (m_Free.size() > MINIMUM_FREE_INDICES)
		{
			index = m_Free.front();
			m_Free.pop_front();
		}
		else
		{
			m_Generation.push_back(0);
			index = static_cast<int>(m_Generation.size() - 1);
		}
		
		return Entity::Make(index, m_Generation[index]);
	}
	
	Entity EntityManager::CreateEntity(Archetype archetype)
	{
		return CreateEntity();
	}

	bool EntityManager::AliveEntity(Entity entity) const
	{
		const int index = entity.Index();
		return m_Generation[index] == entity.Generation();
	}

	void EntityManager::RemoveEntity(Entity entity)
	{
		if (AliveEntity(entity))
		{
			const int index = entity.Index();
			++m_Generation[index];
			m_Free.push_back(index);
			
			m_allocator.free(entity);
		}
	}
	
	void EntityManager::Update(float delta)
	{
		for (System* system : m_Systems)
		{
			system->AssignComponent(this);
			system->Update(this, delta);
		}
	}
}




