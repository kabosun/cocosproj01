#include "MoveSystem.h"
#include "EntityManager.h"

void MoveSystem::AssignComponent(ecs::EntityManager* manager)
{
	Length = manager->GetEntityLength();
	m_Entity = manager->GetEntityPointer();
	m_Position = manager->GetComponentPointer<Position>();
	m_Lifetime = manager->GetComponentPointer<Lifetime>();
	m_Sprite = manager->GetComponentPointer<Sprite>();
}

void MoveSystem::Update(ecs::EntityManager* manager, float delta)
{
	for (int i=0; i<Length; i++)
	{
		Position& position = m_Position[i];
		Lifetime& lifetime = m_Lifetime[i];
		Sprite& sprite = m_Sprite[i];
		
		//position.X += 100 * delta;
		
		log("%d) id:%d life:%d, x:%.2f y:%.2f", i, sprite.Id, lifetime.Value, position.X, position.Y);
		
		lifetime.Value--;
		if (lifetime.Value <= 0)
		{
			manager->RemoveEntity(m_Entity[i]);	// 削除したのでindexがずれてる
		}
	}
}
