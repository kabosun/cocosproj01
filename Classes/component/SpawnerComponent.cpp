#include "SpawnerComponent.h"
#include "TransformComponent.h"
#include "VisualComponent.h"
#include "LifetimeComponent.h"
#include "../ecs/ComponentRegistry.h"

void SpawnerComponent::OnCreate(int index)
{}

void SpawnerComponent::OnRemoveEntity(Entity entity)
{}

void SpawnerComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i=0; i<Size(); i++)
	{
		if (m_Data.SpawnFrame[i].Current >= m_Data.SpawnFrame[i].Max)
		{
			m_Data.SpawnFrame[i].Current = 0;
			
			Archetype archetype = {
				typeid(TransformComponent),
				typeid(LifetimeComponent),
				typeid(VisualComponent),
			};
			
			Entity entity = ComponentRegistry()->CreateEntity(archetype);
			{
				auto&& Transform = ComponentRegistry()->GetComponent<TransformComponent>();
				auto handle = Transform->GetHandle(entity);
				
				float x = mt() % 100;
				float y = mt() % 300;
				Transform->SetPosition(handle, {x, y, 0});
			}
			{
				auto&& Lifetime = ComponentRegistry()->GetComponent<LifetimeComponent>();
				auto handle = Lifetime->GetHandle(entity);
				Lifetime->SetLifetime(handle, {0, 300});
			}
		}
		else
		{
			m_Data.SpawnFrame[i].Current++;
		}
	}
}