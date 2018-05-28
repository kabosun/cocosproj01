#include "LeftComponent.h"
#include "TransformComponent.h"

void LeftComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i = 0; i < Size(); i++)
	{
		auto handle = Transform->GetHandle(GetEntity(i));
		
		float rotation = Transform->GetRotation(handle);
		// 左へ
		float angle = rotation - m_Data.Angle[i] * dt;
		
		Transform->SetRotation(handle, angle);
	}
}

void LeftComponent::GC(const EntityRegistry& registry)
{
	for (int i = 0; i < Size(); i++)
	{
		if (!registry.Alive(GetEntity(i)))
		{
			Remove(i);
		}
	}
}

void LeftComponent::Reset(int index)
{
	m_Data.Angle[index] = 90.f / 180.f * M_PI;
}

void LeftComponent::Compact(int index, int lastIndex)
{
	m_Data.Angle[index] = m_Data.Angle[lastIndex];
}
