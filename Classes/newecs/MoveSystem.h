#pragma once
#include "System.h"
#include "Component.h"

using namespace ecs;

class MoveSystem : public System
{
	Filter filter = EntityManager::CreateFilter(Position::Info(), Lifetime::Info(), Monster::Info());
	int Length = 0;
	ComponentArray<Entity> m_Entity;
	ComponentArray<Position> m_Position;
	ComponentArray<Lifetime> m_Lifetime;
	
public:
	void Init(EntityManager* manager) override;
	
	void AssignComponent(EntityManager* manager) override;
	
	void Update(EntityManager* manager, float delta) override;
};

