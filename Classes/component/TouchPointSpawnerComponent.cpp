#include "TouchPointSpawnerComponent.h"
#include "TransformComponent.h"
#include "LifetimeComponent.h"
#include "../render_component/VisualComponent.h"
#include "../ecs/ComponentRegistry.h"

void TouchPointSpawnerComponent::Update(EntityRegistry& registry, float dt)
{
	assert(Transform);
	assert(Lifetime);
	assert(Visual);
	assert(input);
	
	for (int i=0; i<Size(); i++)
	{
		m_CycleCount++;
		if (m_CycleCount % 2 != 0) continue;
		
		int keyState = input->GetKeyState();
		if ((keyState & (1 << 16)) > 0 || (keyState & (1 << 17)) > 0)
		{
			Vector2f location = input->GetLocation().back();
			Archetype archetype = m_Data.Archetype[i];
			
			Entity entity = ComponentRegistry()->CreateEntity(archetype);
			{
				auto handle = Transform->GetHandle(entity);
				
				float x = location.X;
				float y = location.Y;
				Transform->SetPosition(handle, {x, y});
			}
			{
				auto handle = Lifetime->GetHandle(entity);
				Lifetime->SetLifetime(handle, {0, 8});
			}
			{
				auto handle = Visual->GetHandle(entity);
				Visual->SetTextureName(handle, "IconSet.png");
				Visual->SetRect(handle, 0, 24, 24, 24);
			}
		}
	}
}

