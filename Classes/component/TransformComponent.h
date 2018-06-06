#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;

// コンポーネント
struct _TransformComponent
{
	// ユーザー定義
	std::vector<Vector2f> Position;
	std::vector<float> Rotation;
	std::vector<Vector2f> Scale;
};

class TransformComponent : public ecs2::Component
{
	_TransformComponent m_Data;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Position.resize(maxSize);
		m_Data.Rotation.resize(maxSize);
		m_Data.Scale.resize(maxSize);
	}

	Vector2f GetPosition(ComponentHandle handle) const
	{
		assert(handle.index >= 0);
		return m_Data.Position[handle.index];
	}
	
	void SetPosition(ComponentHandle handle, const Vector2f& position)
	{
		assert(handle.index >= 0);
		m_Data.Position[handle.index] = position;
	}

	float GetRotation(ComponentHandle handle) const
	{
		assert(handle.index >= 0);
		return m_Data.Rotation[handle.index];
	}
	
	void SetRotation(ComponentHandle handle, float rotation)
	{
		m_Data.Rotation[handle.index] = rotation;
	}

	Vector2f GetScale(ComponentHandle handle) const
	{
		return m_Data.Scale[handle.index];
	}
	
	void SetScale(ComponentHandle handle, const Vector2f& scale)
	{
		m_Data.Scale[handle.index] = scale;
	}
	
protected:
	void Reset(int index) override
	{
		m_Data.Position[index].X = 0;
		m_Data.Position[index].Y = 0;
		m_Data.Rotation[index] = M_PI_2;
		m_Data.Scale[index].X = 1;
		m_Data.Scale[index].Y = 1;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Position[index] = m_Data.Position[lastIndex];
		m_Data.Rotation[index] = m_Data.Rotation[lastIndex];
		m_Data.Scale[index] = m_Data.Scale[lastIndex];
	}
};

class TransformFacade final
{
	TransformComponent* component;
	ComponentHandle handle;

public:
	TransformFacade(Entity entity, TransformComponent* component)
	{
		this->component = component;
		this->handle = component->GetHandle(entity);
	}

	Vector2f GetPosition() const
	{
		return component->GetPosition(handle);
	}

	TransformFacade& SetPosition(const Vector2f& position)
	{
		component->SetPosition(handle, position);
		return *this;
	}

	float GetRotation() const
	{
		return component->GetRotation(handle);
	}

	TransformFacade& SetRotation(float rotation)
	{
		component->SetRotation(handle, rotation);
		return *this;
	}

	Vector2f GetScale() const
	{
		return component->GetScale(handle);
	}

	TransformFacade& SetScale(const Vector2f& scale)
	{
		component->SetScale(handle, scale);
		return *this;
	}
};

