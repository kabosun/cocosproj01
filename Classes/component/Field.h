#pragma once

#include <vector>
#include "cocos2d.h"

class Field
{
	int m_Tile;
	std::vector<unsigned short> m_Map;
	std::vector<cocos2d::Sprite*> m_Tiles;
	std::vector<cocos2d::Sprite*> m_Menu;
	cocos2d::Node* m_Root;
	
	cocos2d::Node* m_Scene;
	cocos2d::TMXTiledMap* visual;
	
public:
	void Initialize(cocos2d::Node* scene);
	
	const std::vector<unsigned short> GetMap() const
	{
		return m_Map;
	}
	
	void Dig(float x, float y);
	
	int SelectTile(float x, float y);
};
