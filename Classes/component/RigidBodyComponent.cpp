#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "../ecs/ComponentRegistry.h"

void RigidBodyComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i = 0; i<Size(); i++)
	{
		auto handle = Transform->GetHandle(GetEntity(i));
		Vector3f position = Transform->GetPosition(handle);

		Vector3f& velocity = m_Data.Velocity[i];
		//Vector3f& accelaration = m_Data.Acceleration[i];
		//m_Data.Velocity[i] *= m_Data.Friction[i];

		//velocity += accelaration * dt;
		position += velocity * dt;

		Transform->SetPosition(handle, position);
	}
}

void RigidBodyComponent::OnCreateEntity(Entity entity)
{}

void RigidBodyComponent::OnRemoveEntity(Entity entity)
{}
