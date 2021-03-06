#pragma once


#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;

class TransformComponent;

struct _ProjectileComponent
{
	std::vector<float> Speed;
	std::vector<float> Angle;
};

class ProjectileComponent : public Component, public IUpdatable, public ICollideEventListener
{
	_ProjectileComponent m_Data;
	TransformComponent* Transform;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Speed.resize(maxSize);
		m_Data.Angle.resize(maxSize);
	}
	
	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
	}
	
	void OnCollideEnter(Entity entity1, Entity entity2) override;
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void GC(const EntityRegistry& registry) override;
	
protected:
	void Reset(int index) override;
	
	void Compact(int index, int lastIndex) override;
};
