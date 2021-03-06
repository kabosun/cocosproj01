#include "SpawnerComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "LifetimeComponent.h"
#include "../render_component/VisualComponent.h"
#include "../ecs/ComponentRegistry.h"

void SpawnerComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i=0; i<Size(); i++)
	{
		if (m_Data.SpawnFrame[i].Current >= m_Data.SpawnFrame[i].Max)
		{
			m_Data.SpawnFrame[i].Current = 0;
			Entity entity = GetEntity(i);
			
			auto&& Transform = ComponentRegistry()->GetComponent<TransformComponent>();
			auto handle = Transform->GetHandle(entity);
			auto spawnerPosition = Transform->GetPosition(handle);
			auto&& RigidBody = ComponentRegistry()->GetComponent<RigidBodyComponent>();
			int group = RigidBody->GetGroup(RigidBody->GetHandle(entity));
			
			Archetype archetype = m_Data.Archetype[i];
			
			for (int a=0; a<m_Data.SpawnCount[i]; a++)
			{
				Entity entity = ComponentRegistry()->CreateEntity(archetype);
				{
					auto&& RigidBody = ComponentRegistry()->GetComponent<RigidBodyComponent>();
					auto handle = RigidBody->GetHandle(entity);
					RigidBody->SetGroup(handle, group);
				}
				{
					auto&& Transform = ComponentRegistry()->GetComponent<TransformComponent>();
					auto handle = Transform->GetHandle(entity);
					
					float x = spawnerPosition.X;
					float y = spawnerPosition.Y;
					Transform->SetPosition(handle, {x, y});
				}
				{
					auto&& Lifetime = ComponentRegistry()->GetComponent<LifetimeComponent>();
					auto handle = Lifetime->GetHandle(entity);
					Lifetime->SetLifetime(handle, {0, 180});
				}
			}
		}
		else
		{
			m_Data.SpawnFrame[i].Current++;
		}
	}
}

void SpawnerComponent::GC(const EntityRegistry& registry)
{
	for (int i = 0; i < Size(); i++)
	{
		Entity entity = GetEntity(i);
		if (!registry.Alive(entity))
		{
			Remove(i);
		}
	}
}

void SpawnerComponent::SetArchetype(ComponentHandle handle, Archetype archetype)
{
	m_Data.Archetype[handle.index] = archetype;
}
