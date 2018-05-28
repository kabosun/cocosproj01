#pragma once

#include "cocos2d.h"
#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;

class TransformComponent;

// コンポーネント
struct _VisualComponent
{
	// ユーザー定義
	std::vector<cocos2d::Sprite*> Sprite;
};

class VisualComponent : public ecs2::Component, public IUpdatable, public IEntityEventListener
{
	_VisualComponent m_Data;
	cocos2d::Node* m_Scene;
	TransformComponent* Transform;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize, cocos2d::Node* scene)
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Sprite.resize(maxSize);
		
		m_Scene = scene;
	}
	
	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
	}
	
	void OnCreate(int index) override;
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void OnCreateEntity(Entity entity) override
	{}
	
	void OnRemoveEntity(Entity entity) override;
	
protected:
	void Reset(int index) override
	{
		m_Data.Sprite[index] = nullptr;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Sprite[index] = m_Data.Sprite[lastIndex];
	}
};

