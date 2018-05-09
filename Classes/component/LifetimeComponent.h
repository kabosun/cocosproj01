#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;

using Lifetime = MaxValue<int>;

// コンポーネント
struct _LifetimeComponent
{
	// ユーザー定義
	std::vector<Lifetime> Lifetime;
};

class LifetimeComponent : public Component, public IUpdatable
{
	_LifetimeComponent m_Data;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Lifetime.resize(maxSize);
	}
	
	void Update(EntityRegistry& registry, float dt) override;
	
	Lifetime GetLifetime(ComponentHandle handle)
	{
		return m_Data.Lifetime[handle.index];
	}
	
	void SetLifetime(ComponentHandle handle, const Lifetime& lifetime)
	{
		m_Data.Lifetime[handle.index] = lifetime;
	}
	
protected:
	void Reset(int index) override
	{
		m_Data.Lifetime[index].Current = 10;
		m_Data.Lifetime[index].Max = 10;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Lifetime[index] = m_Data.Lifetime[lastIndex];
	}
};


class LifetimeFacade final
{
	LifetimeComponent* component;
	ComponentHandle handle;
	
public:
	LifetimeFacade(Entity entity, LifetimeComponent* component)
	{
		this->component = component;
		this->handle = component->GetHandle(entity);
	}
	
	Lifetime GetLifetime() const
	{
		return component->GetLifetime(handle);
	}
	
	LifetimeFacade& SetLifetime(const Lifetime& lifetime)
	{
		component->SetLifetime(handle, lifetime);
		return *this;
	}
};

