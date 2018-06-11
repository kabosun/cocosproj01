#pragma once
#include "System.h"
#include "Component.h"

using namespace ecs;

class MoveSystem : public System
{
	int Length = 0;
	Position* m_Position;
	Sprite* m_Sprite;
	
public:
	void AssignComponent(EntityManager* manager) override;
	
	void Update(EntityManager* manager, float delta) override;
};

