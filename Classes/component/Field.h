#pragma once

#include <vector>
#include <functional>
#include "../ecs/Entity.h"

namespace cocos2d
{
	class Node;
	class Sprite;
}

class Field
{
	using FnCreateTile = std::function<void(int, int, int, int)>;
	
	int m_Tile;
	std::vector<int> m_Map;
	std::vector<cocos2d::Sprite*> m_Tiles;
	std::vector<cocos2d::Sprite*> m_Menu;
	std::vector<int> m_Chips;
	FnCreateTile m_OnCreateTile = nullptr;
	
	cocos2d::Node* m_Root;
	
	cocos2d::Node* m_Scene;
	//cocos2d::TMXTiledMap* visual;
	
public:
	void Initialize(cocos2d::Node* scene);
	
	void SetCreateTileFunc(FnCreateTile func)
	{
		m_OnCreateTile = func;
	}
	
	const std::vector<int> GetMap() const
	{
		return m_Map;
	}
	
	const std::vector<cocos2d::Sprite*> GetTiles() const
	{
		return m_Tiles;
	}
	
	int Dig(float x, float y);
	
	int SelectTile(float x, float y);
	
	int GetMap(float x, float y) const;
};
