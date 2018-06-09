#include "PlayerControlComponent.h"
#include "TransformComponent.h"
#include <algorithm>

void PlayerControlComponent::Update(EntityRegistry& registry, float dt)
{
	assert(Transform);
	assert(input);
	
	for (int i = 0; i < Size(); i++)
	{
		//if (!registry.Alive(GetEntity(i))) continue;
		
		auto transformHandle = Transform->GetHandle(GetEntity(i));
		Vector2f position = Transform->GetPosition(transformHandle);
		Vector2f scale = Transform->GetScale(transformHandle);
		
		int keyState = input->GetKeyState();
		Vector2f location;
		auto l = input->GetLocation();
		if (!l.empty()) location = l.back();
		
		int dx = 0;
		int dy = 0;
		
		if ((keyState & (1 << 1)) > 1)
		{
			dx = -1;
		}
		if ((keyState & (1 << 2)) > 1)
		{
			dx = 1;
		}
		if ((keyState & (1 << 3)) > 1)
		{
			dy = 1;
		}
		if ((keyState & (1 << 4)) > 1)
		{
			dy = -1;
		}
#if 1
		if ((keyState & (1 << 16)) > 1 || (keyState & (1 << 17)) > 1)
		{
			if (location.X < position.X)
			{
				dx = -1;
			}
			
			if (location.X > position.X)
			{
				dx = 1;
			}
			
			if (location.Y < position.Y)
			{
				dy = -1;
			}
			
			if (location.Y > position.Y)
			{
				dy = 1;
			}
			
			if (position.X-15 < location.X && position.X +15 > location.X
			&& position.Y-15 < location.Y && position.Y +15 > location.Y)
			{
//				scale.X += 0.1;
//				scale.Y += 0.1;
				
				scale.X = std::min(10.f, scale.X);
				scale.Y = std::min(10.f, scale.Y);
			}
			else
			{
//				scale.X -= 0.05;
//				scale.Y -= 0.05;
				
				scale.X = std::max(1.f, scale.X);
				scale.Y = std::max(1.f, scale.Y);
			}
			//printf("x:%f y:%f x:%f y:%f sx:%f sy:%f\n", position.X, position.Y, location.X, location.Y, scale.X, scale.Y);
		}
#endif
		position.X += m_Data.Speed[i] * dx * dt;
		position.Y += m_Data.Speed[i] * dy * dt;
		
		Transform->SetPosition(transformHandle, position);
		Transform->SetScale(transformHandle, scale);
	}
}

void PlayerControlComponent::GC(const EntityRegistry& registry)
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

void PlayerControlComponent::Reset(int index)
{
	m_Data.Speed[index] = 100;
}

void PlayerControlComponent::Compact(int index, int lastIndex)
{
	m_Data.Speed[index] = m_Data.Speed[lastIndex];
}
