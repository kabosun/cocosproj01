#pragma once

#include <vector>
#include "cocos2d.h"

class Field
{
	int m_Tile;
	std::vector<int> m_Map;
	std::vector<cocos2d::Sprite*> m_Tiles;
	std::vector<cocos2d::Sprite*> m_Menu;
	std::vector<int> m_Chips;
	
	cocos2d::Node* m_Root;
	
	cocos2d::Node* m_Scene;
	cocos2d::TMXTiledMap* visual;
	
public:
	void Initialize(cocos2d::Node* scene);
	
	const std::vector<int> GetMap() const
	{
		return m_Map;
	}
	
	const std::vector<cocos2d::Sprite*> GetTiles() const
	{
		return m_Tiles;
	}
	
	void Dig(float x, float y);
	
	int SelectTile(float x, float y);
	
	int GetMap(float x, float y) const;
};
