#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "Input.h"

using namespace ecs2;

class TransformComponent;

struct _MoveComponent
{
	std::vector<float> Speed;
};

class MoveComponent : public Component, public IUpdatable
{
	_MoveComponent m_Data;
	TransformComponent* Transform = nullptr;
	Input* input = nullptr;

public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);

		m_Data.Speed.resize(maxSize);
	}

	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
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

