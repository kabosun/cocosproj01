#include "MoveComponent.h"
#include "TransformComponent.h"

void MoveComponent::Update(EntityRegistry& registry, float dt)
{
	assert(Transform);
	assert(input);
	
	for (int i = 0; i < Size(); i++)
	{
		//if (!registry.Alive(GetEntity(i))) continue;
		
		auto transformHandle = Transform->GetHandle(GetEntity(i));
		Vector2f position = Transform->GetPosition(transformHandle);
		
		int keyState = input->GetKeyState();
		Vector2f location;
		auto l = input->GetLocation();
		if (!l.empty()) location = l.back();
		
		int d = 0;
		
		if ((keyState & (1 << 1)) > 1)
		{
			d = -1;
		}
		if ((keyState & (1 << 2)) > 1)
		{
			d = 1;
		}
		if ((keyState & (1 << 16)) > 1 || (keyState & (1 << 17)) > 1)
		{
			if (location.X < position.X)
			{
				d = -1;
			}
			
			if (location.X > position.X)
			{
				d = 1;
			}
		}
		
		position.X += m_Data.Speed[i] * dt * d;
		
		Transform->SetPosition(transformHandle, position);
	}
}

void MoveComponent::GC(const EntityRegistry& registry)
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

void MoveComponent::Reset(int index)
{
	m_Data.Speed[index] = 300;
}

void MoveComponent::Compact(int index, int lastIndex)
{
	m_Data.Speed[index] = m_Data.Speed[lastIndex];
}
