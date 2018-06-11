#include "MoveSystem.h"
#include "EntityManager.h"

void MoveSystem::AssignComponent(ecs::EntityManager* manager)
{
	Length = manager->GetEntityLength();
	m_Position = manager->GetComponentPointer<Position>();
	m_Sprite = manager->GetComponentPointer<Sprite>();
}

void MoveSystem::Update(ecs::EntityManager* manager, float delta)
{
	for (int i=0; i<Length; i++)
	{
		Position& position = m_Position[i];
		Sprite& sprite = m_Sprite[i];
		
		//position.X += 100 * delta;
		
		log("%d) x:%f y:%f id:%d", i, position.X, position.Y, sprite.Id);
	}
}
