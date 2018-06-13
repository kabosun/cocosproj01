#include "MoveSystem.h"
#include "EntityManager.h"

void MoveSystem::Init(EntityManager* manager)
{
}

void MoveSystem::AssignComponent(ecs::EntityManager* manager)
{
	ComponentGroup group = manager->GetComponentGroup(filter);
	Length = group.Length();
	m_Entity = group.GetEntityArray();
	m_Position = group.GetComponentArray<Position>();
	m_Lifetime = group.GetComponentArray<Lifetime>();
}

void MoveSystem::Update(ecs::EntityManager* manager, float delta)
{
	if (Length > 0) log("--- update --- %d", Length);
	
	for (int i=0; i<Length; i++)
	{
		Entity& entity = m_Entity[i];
		Position& position = m_Position[i];
		Lifetime& lifetime = m_Lifetime[i];
		//position.X += 100 * delta;
		
		log("%d) id:%d life:%d, x:%.2f y:%.2f", i, entity.Id, lifetime.Value, position.X, position.Y);
		
		lifetime.Value--;
		if (lifetime.Value <= 0)
		{
			manager->RemoveEntity(entity);
		}
	}
}

//const int Health::Size = sizeof(Health);
