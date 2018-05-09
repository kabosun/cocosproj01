#include "HealthComponent.h"
#include "../ecs/ComponentRegistry.h"

void HealthComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i=0; i<Size(); i++)
	{
		Health& health = m_Data.Health[i];
		if (health.Current <= 0)
		{
			// Destory Entity
			registry.Remove(GetEntity(i));
		}
	}
}
