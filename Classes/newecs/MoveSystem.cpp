#include "MoveSystem.h"
#include "EntityManager.h"

void MoveSystem::AssignComponent(ecs::EntityManager* manager)
{
	Length = manager->length();
	m_Position = manager->GetComponentHead<Position>();
	m_Sprite = manager->GetComponentHead<Sprite>();
}

void MoveSystem::Update(ecs::EntityManager* manager, float delta)
{
	for (int i=0; i<Length; i++)
	{
		Position& position = m_Position[i];
		Sprite& sprite = m_Sprite[i];
		
		//position.X += 100 * delta;
		
		printf("%d) x:%f y:%f id:%d\n", i, position.X, position.Y, sprite.Id);
	}
}
