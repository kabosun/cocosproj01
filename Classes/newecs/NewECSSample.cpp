#include "NewECSSample.h"
#include "MoveSystem.h"
#include "EntityManager.h"

using namespace ecs;

void NewECSSample::Init()
{
	manager.Init();
	manager.RegisterSystem<MoveSystem>();
	
	Archetype archetype1 = manager.CreateArchetype(Position::Info(), Lifetime::Info(), Monster::Info(), Sprite::Info());
	log("archetype1: %zu", archetype1.to_ulong());
	log("archetype1: %s", archetype1.to_string().c_str());

	Archetype archetype2 = manager.CreateArchetype(Position::Info(), Lifetime::Info());
	log("archetype2: %zu", archetype2.to_ulong());
	log("archetype2: %s", archetype2.to_string().c_str());
	
	std::vector<Entity> list;
	for (int i=0; i<5; i++)
	{
		Entity entity = manager.CreateEntity(archetype1);
		list.push_back(entity);
		
		manager.AssignComponent<Position>(entity, 100+i, 150+i);
		manager.AssignComponent<Lifetime>(entity, 3+i);
		manager.AssignComponent<Sprite>(entity, 1+i);
	}

	for (int i = 0; i<1; i++)
	{
		Entity entity = manager.CreateEntity(archetype2);
		list.push_back(entity);

		manager.AssignComponent<Position>(entity, 200 + i, 250 + i);
		manager.AssignComponent<Lifetime>(entity, 3 + i);
	}
	
	manager.RemoveEntity(list[0]);
	manager.RemoveEntity(list[4]);
}

void NewECSSample::Update(float delta)
{
	manager.Update(delta);
	manager.GC();
}






