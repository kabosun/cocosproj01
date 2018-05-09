#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include <iostream>

using namespace ecs2;

// コンポーネント
struct _ExplodeComponent
{
	// ユーザー定義
	std::vector<int> ExplodePerticileId;
};


class ExplodeComponent : public Component, public IEntityEventListener
{
	class TransformComponent* m_TransformComponent = nullptr;
	_ExplodeComponent m_Data;
	
public:
	
	void Initialize(EntityRegistry& registry, int maxSize) override;
	
	void OnCreateEntity(Entity entity) override;
	
	void OnRemoveEntity(Entity entity) override;

	int GetExplodePerticileId(ComponentHandle handle)
	{
		return m_Data.ExplodePerticileId[handle.index];
	}
	
protected:
	void Reset(int index) override
	{
		m_Data.ExplodePerticileId[index] = 0;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.ExplodePerticileId[index] = m_Data.ExplodePerticileId[lastIndex];
	}
};

