#include "MoveComponent.h"
#include "TransformComponent.h"

void MoveComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i = 0; i < Size(); i++)
	{
		//if (!registry.Alive(GetEntity(i))) continue;
		
		auto handle = Transform->GetHandle(GetEntity(i));
		
		float rotation = Transform->GetRotation(handle);
		Vector2f position = Transform->GetPosition(handle);
		
		// 前進する
		float v = (m_Data.Speed[i] * dt);
		position.X += cos(rotation) * v;
		position.Y += sin(rotation) * v;

		Transform->SetPosition(handle, position);
		
		// 左へ
		//Transform->SetRotation(handle, rotation - (dt * 180.f / 360 * M_2_PI));
	}
}

void MoveComponent::GC(const EntityRegistry& registry)
{
	for (int i = 0; i < Size(); i++)
	{
		if (!registry.Alive(GetEntity(i)))
		{
			Remove(i);
		}
	}
}

void MoveComponent::Reset(int index)
{
	m_Data.Speed[index] = 300;
}

void MoveComponent::Compact(int index, int lastIndex)
{
	m_Data.Speed[index] = m_Data.Speed[lastIndex];
}
