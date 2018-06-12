#pragma once
#include "System.h"
#include "Component.h"

using namespace ecs;

class MoveSystem : public System
{
	int Length = 0;
	Entity* m_Entity;
	Position* m_Position;
	Lifetime* m_Lifetime;
	Sprite* m_Sprite;
	
public:
	void AssignComponent(EntityManager* manager) override;
	
	void Update(EntityManager* manager, float delta) override;
};

