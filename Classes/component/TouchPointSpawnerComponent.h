#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "Input.h"

using namespace ecs2;

class TransformComponent;
class LifetimeComponent;
class VisualComponent;

// コンポーネント
struct _TouchPointSpawnerComponent
{
	// ユーザー定義
	std::vector<Archetype> Archetype;
};

class TouchPointSpawnerComponent : public Component, public IUpdatable
{
	_TouchPointSpawnerComponent m_Data;
	Input* input = nullptr;
	TransformComponent* Transform = nullptr;
	LifetimeComponent* Lifetime = nullptr;
	VisualComponent* Visual = nullptr;
	int m_CycleCount = 0;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Archetype.resize(maxSize);
	}
	
	void SetInput(Input* input)
	{
		this->input = input;
	}
	
	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
	}
	
	void SetSharedComponent(LifetimeComponent* lifetime)
	{
		Lifetime = lifetime;
	}
	
	void SetSharedComponent(VisualComponent* visual)
	{
		Visual = visual;
	}
	
	void SetArchetype(ComponentHandle handle, Archetype archetype)
	{
		m_Data.Archetype[handle.index] = archetype;
	}
	
	void Update(EntityRegistry& registry, float dt) override;
	
protected:
	void Reset(int index) override
	{
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Archetype[index] = m_Data.Archetype[lastIndex];
	}
};

