//
//  VisualComponent.cpp
//  MyGame01
//
//  Created by gladius on 2018/05/09.
//

#include "VisualComponent.h"
#include "TransformComponent.h"
#include "../ecs/ComponentRegistry.h"

USING_NS_CC;

void VisualComponent::OnCreate(int index)
{
	DrawNode* node = DrawNode::create();
	m_Data.DrawNode[index] = node;
	
	node->drawDot(Vec2::ZERO, 50, Color4F::YELLOW);
	node->setPosition(Vec2(0, 0));
	
	m_Scene->addChild(node);
}

void VisualComponent::OnRemoveEntity(Entity entity)
{
	auto handle = GetHandle(entity);
	m_Scene->removeChild(m_Data.DrawNode[handle.index]);
	m_Data.DrawNode[handle.index] = nullptr;
}

void VisualComponent::Update(EntityRegistry& registry, float dt)
{
	auto transform = ComponentRegistry()->GetComponent<TransformComponent>();
	
	for (int i=0; i<Size(); i++)
	{
		Entity entity = GetEntity(i);
		if (registry.Alive(entity))
		{
			auto handle = transform->GetHandle(entity);
			auto position = transform->GetPosition(handle);
			
			m_Data.DrawNode[i]->setPosition(position.X, position.Y);
			
			position.X += 100 * dt;
			transform->SetPosition(handle, position);
		}
	}
}
