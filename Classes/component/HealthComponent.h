#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;

using Health = MaxValue<int>;

// コンポーネント
struct _HealthComponent
{
	// ユーザー定義
	std::vector<Health> Health;
};

class HealthComponent : public Component, public IUpdatable
{
	_HealthComponent m_Data;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Health.resize(maxSize);
	}
	
	void Update(EntityRegistry& registry, float dt) override;
	
	Health GetHealth(ComponentHandle handle) const
	{
		return m_Data.Health[handle.index];
	}

	void SetHealth(ComponentHandle handle, const Health& health)
	{
		m_Data.Health[handle.index] = health;
	}
	
protected:
	void Reset(int index) override
	{
		m_Data.Health[index].Current = 10;
		m_Data.Health[index].Max = 10;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Health[index] = m_Data.Health[lastIndex];
	}
};
