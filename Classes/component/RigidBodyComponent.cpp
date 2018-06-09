#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "../ecs/ComponentRegistry.h"

void RigidBodyComponent::OnCreate(int index)
{
	assert(Transform);
	
	Entity entity = GetEntity(index);
	auto handle = Transform->GetHandle(entity);
	auto position = Transform->GetPosition(handle);
	
	m_Data.RigidBody[index].entity = entity;
	m_Data.RigidBody[index].shape = std::make_shared<RectShape>(position.X, position.Y, 16, 16);
	physics->AddBody(m_Data.RigidBody[index]);
}

void RigidBodyComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i = 0; i<Size(); i++)
	{
	}
}

void RigidBodyComponent::GC(const EntityRegistry& registry)
{
	for (int i = 0; i < Size(); i++)
	{
		Entity entity = GetEntity(i);
		if (!registry.Alive(entity))
		{
			physics->RemoveBody(entity);
			Remove(i);
		}
	}
}
