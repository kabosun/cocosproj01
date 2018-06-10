#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "Input.h"

using namespace ecs2;

class RigidBodyComponent;

struct _PlayerControlComponent
{
	std::vector<float> Speed;
};

class PlayerControlComponent : public Component, public IUpdatable
{
	_PlayerControlComponent m_Data;
	RigidBodyComponent* RigidBody = nullptr;
	Input* input = nullptr;

public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);

		m_Data.Speed.resize(maxSize);
	}

	void SetSharedComponent(RigidBodyComponent* rigidBody)
	{
		RigidBody = rigidBody;
	}
	
	void SetInput(Input* input)
	{
		this->input = input;
	}

	void Update(EntityRegistry& registry, float dt) override;
	
	void GC(const EntityRegistry& registry) override;

protected:
	void Reset(int index) override;

	void Compact(int index, int lastIndex) override;
};

