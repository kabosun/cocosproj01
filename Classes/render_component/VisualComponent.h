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

class VisualComponent : public ecs2::Component, public IUpdatable
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
	
	
protected:
	void Reset(int index) override
	{
		m_Data.TextureName[index] = "chip02d_dungeon.png";
		int chip = 443;
		m_Data.Rect[index] = {(float)(chip%30*16), (float)(chip/30*16), 16, 16};
		m_Data.Sprite[index] = nullptr;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.TextureName[index] = m_Data.TextureName[lastIndex];
		m_Data.Rect[index] = m_Data.Rect[lastIndex];
		m_Data.Sprite[index] = m_Data.Sprite[lastIndex];
		
		m_Data.Sprite[lastIndex] = nullptr;		// 参照カウンタなので参照は残さない
	}
};

