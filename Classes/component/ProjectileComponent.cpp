#include "ProjectileComponent.h"
#include "TransformComponent.h"
#include "../ecs/ComponentRegistry.h"

void ProjectileComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i = 0; i < Size(); i++)
	{
		auto handle = Transform->GetHandle(GetEntity(i));
		
		float rotation = Transform->GetRotation(handle);
		Vector2f position = Transform->GetPosition(handle);
		
		// 前進する
		float v = (m_Data.Speed[i] * dt);
		position.X += cos(rotation) * v;
		position.Y += sin(rotation) * v;
		
		Transform->SetPosition(handle, position);
	}
}

void ProjectileComponent::OnCollideEnter(Entity entity1, Entity entity2)
{
	if (!HasComponent(entity1)) return;
	
	printf("hit! 1:%d 2:%d\n", entity1.Index(), entity2.Index());
	ComponentRegistry()->RemoveEntity(entity1);
}

void ProjectileComponent::GC(const EntityRegistry& registry)
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

void ProjectileComponent::Reset(int index)
{
	m_Data.Speed[index] = 300;
	m_Data.Angle[index] = 90.f / 180.f * M_PI;
}

void ProjectileComponent::Compact(int index, int lastIndex)
{
	m_Data.Speed[index] = m_Data.Speed[lastIndex];
	m_Data.Angle[index] = m_Data.Angle[lastIndex];
}
