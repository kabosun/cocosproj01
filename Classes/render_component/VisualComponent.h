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
	std::vector<std::string> TextureName;
	std::vector<cocos2d::Rect> Rect;
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
		
		m_Data.TextureName.resize(maxSize);
		m_Data.Rect.resize(maxSize);
		m_Data.Sprite.resize(maxSize);
		
		m_Scene = scene;
	}
	
	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
	}
	
	void SetTextureName(ComponentHandle handle, const std::string& name)
	{
		m_Data.TextureName[handle.index] = name;
	}
	
	void SetRect(ComponentHandle handle, int x, int y, int w, int h)
	{
		m_Data.Rect[handle.index].setRect(x, y, w, h);
	}
	
	void OnCreate(int index) override;
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void GC(const EntityRegistry& registry) override;
	
	void OnCreateEntity(Entity entity) override
	{}
	
	void OnRemoveEntity(Entity entity) override;
	
protected:
	void Reset(int index) override
	{
		m_Data.TextureName[index] = "TileA1.png";
		m_Data.Rect[index] = {0, 0, 32, 32};
		m_Data.Sprite[index] = nullptr;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.TextureName[index] = m_Data.TextureName[lastIndex];
		m_Data.Rect[index] = m_Data.Rect[lastIndex];
		m_Data.Sprite[index] = m_Data.Sprite[lastIndex];
	}
};

