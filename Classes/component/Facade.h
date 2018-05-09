#pragma once

/**
 * 便利関数
 */
#include "ECS2.h"

using namespace ecs2;

class EntityFacade final
{
	Entity entity;
	ComponentRegistry* registry;

public:
	EntityFacade(ComponentRegistry* registry, Entity entity)
	{
		this->entity = entity;
		this->registry = registry;
	}

	TransformFacade Transform()
	{
		auto&& component = registry->GetComponent<TransformComponent>();
		return TransformFacade(entity, component.get());
	}

	RigidBodyFacade Physics()
	{
		auto&& component = registry->GetComponent<RigidBodyComponent>();
		return RigidBodyFacade(entity, component.get());
	}

	HealthFacade Health()
	{
		auto&& component = registry->GetComponent<HealthComponent>();
		return HealthFacade(entity, component.get());
	}
	
	LifetimeFacade Lifetime()
	{
		auto&& component = registry->GetComponent<LifetimeComponent>();
		return LifetimeFacade(entity, component.get());
	}
};
