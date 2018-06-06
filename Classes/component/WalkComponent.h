#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "Field.h"
#include "cocos2d.h"

using namespace ecs2;

class TransformComponent;

// コンポーネント
struct _WalkComponent
{
	// ユーザー定義
	std::vector<int> Direction;
	std::vector<float> Speed;
};

class WalkComponent : public Component, public IUpdatable
{
	_WalkComponent m_Data;
	Field* field = nullptr;
	TransformComponent* Transform = nullptr;
	std::vector<float> m_Evaluation;
	std::vector<bool> m_History;
	int m_ReevaluateFrame = 0;
	std::vector<cocos2d::Label*> m_Sprites;
	
public:
	void Initialize(EntityRegistry& registry, int maxSize) override
	{
		Component::Initialize(registry, maxSize);
		
		m_Data.Direction.resize(maxSize);
		m_Data.Speed.resize(maxSize);
	}
	
	void SetField(Field* field, cocos2d::Node* scene)
	{
		this->field = field;
		
		m_Evaluation.resize(field->GetMap().size());
		m_History.resize(field->GetMap().size(), 0);
		m_Sprites.resize(field->GetMap().size());
		
		auto tiles = field->GetTiles();
		for (int i=0; i<m_Sprites.size(); i++)
		{
			m_History[i] = 0;
			auto a = std::to_string((int)m_History[i]);
			
			m_Sprites[i] = cocos2d::Label::createWithSystemFont(a, "HiraKakuProN-W6", 10);
			m_Sprites[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
//			tiles[i]->addChild(m_Sprites[i]);
		}
	}
	
	void SetSharedComponent(TransformComponent* transform)
	{
		Transform = transform;
	}
	
	void Update(EntityRegistry& registry, float dt) override;
	
protected:
	void Reset(int index) override
	{
		m_Data.Direction[index] = 0;	// right
		m_Data.Speed[index] = 100;
	}
	
	void Compact(int index, int lastIndex) override
	{
		m_Data.Direction[index] = m_Data.Direction[lastIndex];
		m_Data.Speed[index] = m_Data.Speed[lastIndex];
	}
};

