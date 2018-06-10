#include "NewECSSample.h"
#include "MoveSystem.h"

using namespace ecs;

void NewECSSample::Init()
{
	manager.Init();
	manager.RegisterSystem<MoveSystem>();
	
	std::vector<Entity> list;
	for (int i=0; i<5; i++)
	{
		Entity entity = manager.CreateEntity();
		list.push_back(entity);
		
		manager.AssignComponent<Position>(entity, 100+i, 150+i);
		manager.AssignComponent<Sprite>(entity, 1+i);
	}
	
	manager.RemoveEntity(list[0]);
	manager.RemoveEntity(list[4]);
	
	manager.CreateArchetype<Position, Sprite>();
}

void NewECSSample::Update(float delta)
{
	manager.Update(delta);
}






