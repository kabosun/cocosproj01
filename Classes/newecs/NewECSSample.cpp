#include "NewECSSample.h"
#include "MoveSystem.h"
#include "EntityManager.h"

using namespace ecs;

void NewECSSample::Init()
{
	manager.Init();
	manager.RegisterSystem<MoveSystem>();
	
	Archetype archetype = manager.CreateArchetype(Position::Info(), Lifetime::Info(), Sprite::Info());
	
	log("%zu", archetype.to_ulong());
	log("%s", archetype.to_string().c_str());
	
	std::vector<Entity> list;
	for (int i=0; i<5; i++)
	{
		Entity entity = manager.CreateEntity(archetype);
		list.push_back(entity);
		
		manager.AssignComponent<Position>(entity, 100+i, 150+i);
		manager.AssignComponent<Lifetime>(entity, 3+i);
		manager.AssignComponent<Sprite>(entity, 1+i);
	}
	
	manager.RemoveEntity(list[0]);
	manager.RemoveEntity(list[4]);
	
//	const std::type_index type = typeid(requires<Position, Sprite, Position, Sprite, Position, Sprite, Position, Sprite>);
//	log("type size: %zu", sizeof(type));
//	log("type size: %zu", sizeof(requires<Position, Sprite, Position, Sprite, Position, Sprite, Position, Sprite>));
//	log("type size: %zu", sizeof(int));
}

void NewECSSample::Update(float delta)
{
	manager.Update(delta);
}






