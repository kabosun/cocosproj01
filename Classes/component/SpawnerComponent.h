#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;


// コンポーネント
struct _SpawnerComponent
{
	// ユーザー定義
	std::vector<Archetype> Archetype;
	std::vector<MaxValue<int>> SpawnFrame;
	std::vector<int> SpawnCount;
};

class SpawnerComponent : public Component, public IUpdatable
{
	_SpawnerComponent m_Data;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Archetype.resize(maxSize);
		m_Data.SpawnFrame.resize(maxSize);
		m_Data.SpawnCount.resize(maxSize);
	}
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void GC(const EntityRegistry& registry) override;
	
	void SetArchetype(ComponentHandle handle, Archetype archtype);
	
protected:
	void Reset(int index) override
	{
		m_Data.SpawnFrame[index].Current = 0;
		m_Data.SpawnFrame[index].Max = 180;
		m_Data.SpawnCount[index] = 1;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Archetype[index] = m_Data.Archetype[lastIndex];
		m_Data.SpawnFrame[index] = m_Data.SpawnFrame[lastIndex];
		m_Data.SpawnCount[index] = m_Data.SpawnCount[lastIndex];
	}
};

