//
//  VisualComponent.cpp
//  MyGame01
//
//  Created by gladius on 2018/05/09.
//

#include "VisualComponent.h"
#include "cocos2d.h"
#include "../ecs/ComponentRegistry.h"
#include "../component/TransformComponent.h"

USING_NS_CC;

void VisualComponent::OnCreate(int index)
{
//	Sprite* sprite = Sprite::create("mon_001r.png", Rect(6, 6, 18, 17));
	Sprite* sprite = Sprite::create(m_Data.TextureName[index]);
	game::Rect& rect = m_Data.Rect[index];
	sprite->setTextureRect(CC_RECT_PIXELS_TO_POINTS(rect));
	sprite->setContentSize(cocos2d::Size(rect.size.width, rect.size.height));
	m_Data.Sprite[index] = sprite;
	m_Scene->addChild(sprite);
}

void VisualComponent::Update(EntityRegistry& registry, float dt)
{
	for (int i=0; i<Size(); i++)
	{
		Entity entity = GetEntity(i);
		if (registry.Alive(entity))
		{
			auto handle = Transform->GetHandle(entity);
			auto position = Transform->GetPosition(handle);
			auto scale = Transform->GetScale(handle);
			
			auto&& sprite = m_Data.Sprite[i];
			sprite->setPosition(position.X, position.Y);
			sprite->setScale(scale.X, scale.Y);
		}
	}
}

void VisualComponent::GC(const EntityRegistry& registry)
{
	for (int i = 0; i < Size(); i++)
	{
		Entity entity = GetEntity(i);
		if (!registry.Alive(entity))
		{
			m_Scene->removeChild(m_Data.Sprite[i]);
			m_Data.Sprite[i] = nullptr;
			
			Remove(i);
		}
	}
}

void VisualComponent:: SetRect(ComponentHandle handle, int x, int y, int w, int h)
{
	game::Rect& rect = m_Data.Rect[handle.index];
	rect.origin.x = x;
	rect.origin.y = y;
	rect.size.width = w;
	rect.size.height = h;
}

void VisualComponent::Reset(int index)
{
	m_Data.TextureName[index] = "tile.png";
	int chip = 8;
	m_Data.Rect[index] = game::Rect(chip % 8 * 16, chip / 8 * 16, 16, 16);
	m_Data.Sprite[index] = nullptr;
}

void VisualComponent::Compact(int index, int lastIndex)
{
	m_Data.TextureName[index] = m_Data.TextureName[lastIndex];
	m_Data.Rect[index] = m_Data.Rect[lastIndex];
	m_Data.Sprite[index] = m_Data.Sprite[lastIndex];

	m_Data.Sprite[lastIndex] = nullptr;		// 参照カウンタなので参照は残さない
}
