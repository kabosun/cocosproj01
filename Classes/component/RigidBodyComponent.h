#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include <iostream>

using namespace ecs2;

class TransformComponent;

// コンポーネント
struct _RigidBodyComponent
{
	// ユーザー定義
	std::vector<Vector2f> Velocity;
	std::vector<Vector2f> Acceleration;
	std::vector<float> Friction;
};


class RigidBodyComponent : public Component, public IUpdatable, public IEntityEventListener
{
	_RigidBodyComponent m_Data;
	TransformComponent* Transform;
	
public:
	
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Velocity.resize(maxSize);
		m_Data.Acceleration.resize(maxSize);
		m_Data.Friction.resize(maxSize);
	}

	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
	}
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void OnCreateEntity(Entity entity) override;
	
	void OnRemoveEntity(Entity entity) override;

	Vector2f GetVelocity(ComponentHandle handle)
	{
		return m_Data.Velocity[handle.index];
	}

	void SetVelocity(ComponentHandle handle, const Vector2f& velocity)
	{
		m_Data.Velocity[handle.index] = velocity;
	}

	Vector2f GetAcceleration(ComponentHandle handle)
	{
		return m_Data.Acceleration[handle.index];
	}

	void SetAcceleration(ComponentHandle handle, const Vector2f& acceleration)
	{
		m_Data.Acceleration[handle.index] = acceleration;
	}
	
	float GetFriction(ComponentHandle handle)
	{
		return m_Data.Friction[handle.index];
	}
	
	void SetFriction(ComponentHandle handle, float friction)
	{
		m_Data.Friction[handle.index] = friction;
	}
	
protected:
	void Reset(int index) override
	{
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Velocity[index] = m_Data.Velocity[lastIndex];
		m_Data.Acceleration[index] = m_Data.Acceleration[lastIndex];
	}
};

class RigidBodyFacade final
{
	RigidBodyComponent* component;
	Entity entity;

public:
	RigidBodyFacade(Entity entity, RigidBodyComponent* component)
	{
		this->entity = entity;
		this->component = component;
	}
	
	Vector2f GetVelocity() const
	{
		ComponentHandle handle = component->GetHandle(entity);
		return component->GetVelocity(handle);
	}
	
	void SetVelocity(const Vector2f& velocity)
	{
		ComponentHandle handle = component->GetHandle(entity);
		auto&& value = component->GetVelocity(handle);
		value = velocity;
	}
	
	Vector2f GetAcceleration() const
	{
		ComponentHandle handle = component->GetHandle(entity);
		return component->GetAcceleration(handle);
	}
	
	void SetAcceleration(const Vector2f& acceleration)
	{
		ComponentHandle handle = component->GetHandle(entity);
		component->SetAcceleration(handle, acceleration);
	}
};

