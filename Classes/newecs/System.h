#pragma once
#include "EntityManager.h"

namespace ecs
{
	class System
	{
	public:
		virtual void AssignComponent(ecs::EntityManager* manager) = 0;
		virtual void Update(EntityManager* manager, float delta);
	};
}
