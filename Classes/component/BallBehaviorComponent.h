#pragma once

#include <array>
#include <iostream>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;

class BallBehaviorComponent : public Component, public IUpdatable, public IEntityEventListener
{
public:
	
	void Initialize(EntityRegistry& registry, int maxSize) override;
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void OnCreateEntity(Entity entity) override;
	
	void OnRemoveEntity(Entity entity) override;
	
	void OnContact(Entity body1, Entity body2);
	
protected:
	void Reset(int index) override
	{
	}
	
	void Compact(int index, int lastIndex) override
	{
	}
};

