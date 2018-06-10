#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include <iostream>
#include "Physics.h"

using namespace ecs2;

class TransformComponent;

// コンポーネント
struct _RigidBodyComponent
{
	// ユーザー定義
	std::vector<RigidBody*> RigidBody;
};


class RigidBodyComponent : public Component, public IUpdatable
{
	_RigidBodyComponent m_Data;
	TransformComponent* Transform;
	Physics* physics;
	
public:
	
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.RigidBody.resize(maxSize);
	}

	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
	}
	
	void SetPhysics(Physics* physics)
	{
		this->physics = physics;
	}
	
	void OnCreate(int index) override;
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void GC(const EntityRegistry& registry) override;

	int GetGroup(ComponentHandle handle)
	{
		return m_Data.RigidBody[handle.index]->Group;
	}
	
	void SetGroup(ComponentHandle handle, int group)
	{
		m_Data.RigidBody[handle.index]->Group = group;
	}
	
	Vector2f GetVelocity(ComponentHandle handle)
	{
		return m_Data.RigidBody[handle.index]->Velocity;
	}

	void SetVelocity(ComponentHandle handle, Vector2f velocity)
	{
		m_Data.RigidBody[handle.index]->Velocity = velocity;
	}

	Vector2f GetAcceleration(ComponentHandle handle)
	{
		return m_Data.RigidBody[handle.index]->Acceleration;
	}

	void SetAcceleration(ComponentHandle handle, Vector2f acceleration)
	{
		m_Data.RigidBody[handle.index]->Acceleration = acceleration;
	}
	
	float GetFriction(ComponentHandle handle)
	{
		return m_Data.RigidBody[handle.index]->Friction;
	}
	
	void SetFriction(ComponentHandle handle, float friction)
	{
		m_Data.RigidBody[handle.index]->Friction = friction;
	}
	
protected:
	void Reset(int index) override
	{
		m_Data.RigidBody[index] = nullptr;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.RigidBody[index] = m_Data.RigidBody[lastIndex];
	}
};
