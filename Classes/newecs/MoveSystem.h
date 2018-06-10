#pragma once
#include "System.h"
#include "Component.h"

using namespace ecs;

class MoveSystem : public ecs::System
{
	int Length = 0;
	Position* m_Position;
	Sprite* m_Sprite;
	
public:
	void AssignComponent(ecs::EntityManager* manager) override;
	
	void Update(ecs::EntityManager* manager, float delta) override;
};

