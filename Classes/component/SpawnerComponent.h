#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;


// コンポーネント
struct _SpawnerComponent
{
	// ユーザー定義
	std::vector<MaxValue<int>> SpawnFrame;
	std::vector<int> SpawnCount;
};

class SpawnerComponent : public Component, public IUpdatable, public IEntityEventListener
{
	_SpawnerComponent m_Data;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.SpawnFrame.resize(maxSize);
		m_Data.SpawnCount.resize(maxSize);
	}
	
	void OnCreate(int index) override;
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void OnCreateEntity(Entity entity) override
	{}
	
	void OnRemoveEntity(Entity entity) override;
	
protected:
	void Reset(int index) override
	{
		m_Data.SpawnFrame[index].Current = 0;
		m_Data.SpawnFrame[index].Max = 1;
		m_Data.SpawnCount[index] = 4;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.SpawnFrame[index] = m_Data.SpawnFrame[lastIndex];
	}
};

