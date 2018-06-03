#include "SpawnerComponent.h"
#include "TransformComponent.h"
#include "LifetimeComponent.h"
#include "../render_component/VisualComponent.h"
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
			
			auto&& Transform = ComponentRegistry()->GetComponent<TransformComponent>();
			auto handle = Transform->GetHandle(GetEntity(i));
			auto position = Transform->GetPosition(handle);
			
			Archetype archetype = m_Data.Archetype[i];
			
			for (int a=0; a<m_Data.SpawnCount[i]; a++)
			{
				Entity entity = ComponentRegistry()->CreateEntity(archetype);
				{
					auto&& Transform = ComponentRegistry()->GetComponent<TransformComponent>();
					auto handle = Transform->GetHandle(entity);
					
					float x = position.X + mt() % 100;
					float y = position.Y + mt() % 100 + 100;
					Transform->SetPosition(handle, {x, y});
				}
				{
					auto&& Lifetime = ComponentRegistry()->GetComponent<LifetimeComponent>();
					auto handle = Lifetime->GetHandle(entity);
					Lifetime->SetLifetime(handle, {0, 256});
				}
			}
		}
		else
		{
			m_Data.SpawnFrame[i].Current++;
		}
	}
}

void SpawnerComponent::SetArchetype(ComponentHandle handle, Archetype archetype)
{
	m_Data.Archetype[handle.index] = archetype;
}
