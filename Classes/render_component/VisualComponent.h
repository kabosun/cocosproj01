#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

using namespace ecs2;

class TransformComponent;

namespace cocos2d
{
	class Node;
	class Sprite;
}

// コンポーネント
struct _VisualComponent
{
	// ユーザー定義
	std::vector<std::string> TextureName;
	std::vector<game::Rect> Rect;
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
	
	void SetRect(ComponentHandle handle, int x, int y, int w, int h);
	
	void OnCreate(int index) override;
	
	void Update(EntityRegistry& registry, float dt) override;
	
	void GC(const EntityRegistry& registry) override;
	
	
protected:
	void Reset(int index) override;
	
	void Compact(int index, int lastIndex) override;
};

