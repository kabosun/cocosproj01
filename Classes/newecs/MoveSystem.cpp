#include "MoveSystem.h"
#include "EntityManager.h"

void MoveSystem::AssignComponent(ecs::EntityManager* manager)
{
	Archetype filter = manager->CreateFilter(Position::Info(), Lifetime::Info());
	ComponentPack components = manager->GetComponentPack(filter);

	Length = components.Length();
	m_Entity = components.GetEntityArray();
	m_Position = components.GetComponentArray<Position>();
	m_Lifetime = components.GetComponentArray<Lifetime>();
}

void MoveSystem::Update(ecs::EntityManager* manager, float delta)
{
	for (int i=0; i<Length; i++)
	{
		Position& position = m_Position[i];
		Lifetime& lifetime = m_Lifetime[i];
		
		//position.X += 100 * delta;
		
		log("%d) id:%d life:%d, x:%.2f y:%.2f", i, m_Entity[i].Id, lifetime.Value, position.X, position.Y);
		
		lifetime.Value--;
		if (lifetime.Value <= 0)
		{
			manager->RemoveEntity(m_Entity[i]);	// 削除したのでindexがずれてる
		}
	}
}
