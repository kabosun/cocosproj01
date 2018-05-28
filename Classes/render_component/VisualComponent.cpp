//
//  VisualComponent.cpp
//  MyGame01
//
//  Created by gladius on 2018/05/09.
//

#include "VisualComponent.h"
#include "../component/TransformComponent.h"
#include "../component/LifetimeComponent.h"
#include "../ecs/ComponentRegistry.h"

USING_NS_CC;

void VisualComponent::OnCreate(int index)
{
	Sprite* sprite = Sprite::create("mon_001r.png", Rect(6, 6, 18, 17));
	m_Data.Sprite[index] = sprite;
	m_Scene->addChild(sprite);
}

void VisualComponent::OnRemoveEntity(Entity entity)
{
	if (HasComponent(entity))
	{
		auto handle = GetHandle(entity);
		
		m_Scene->removeChild(m_Data.Sprite[handle.index]);
		m_Data.Sprite[handle.index] = nullptr;
		
		Remove(handle.index);
	}
}

void VisualComponent::Update(EntityRegistry& registry, float dt)
{
	auto transform = ComponentRegistry()->GetComponent<TransformComponent>();
	auto lifetime = ComponentRegistry()->GetComponent<LifetimeComponent>();
	
	for (int i=0; i<Size(); i++)
	{
		Entity entity = GetEntity(i);
		if (registry.Alive(entity))
		{
			auto handle = transform->GetHandle(entity);
			auto position = transform->GetPosition(handle);
			
			m_Data.Sprite[i]->setPosition(position.X, position.Y);

			if (lifetime != nullptr)
			{
				auto handle = lifetime->GetHandle(entity);
				auto life = lifetime->GetLifetime(handle);

				m_Data.Sprite[i]->setScale(life.Current * 1.0f / life.Max * 3);
			}
		}
	}
}
