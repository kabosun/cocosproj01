#include "LeftComponent.h"
#include "TransformComponent.h"

void LeftComponent::Update(EntityRegistry& registry, float dt)
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
		
		
		// 左へ
		float angle = rotation - m_Data.Angle[i] * dt;
		
		Transform->SetRotation(handle, angle);
	}
}

void LeftComponent::GC(const EntityRegistry& registry)
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

void LeftComponent::Reset(int index)
{
	m_Data.Speed[index] = 300;
	m_Data.Angle[index] = 90.f / 180.f * M_PI;
}

void LeftComponent::Compact(int index, int lastIndex)
{
	m_Data.Speed[index] = m_Data.Speed[lastIndex];
	m_Data.Angle[index] = m_Data.Angle[lastIndex];
}
