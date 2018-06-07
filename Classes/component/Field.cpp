#include "Field.h"

USING_NS_CC;

static const int SCALE = 2;
static const int TILECHIP_SIZE = 16;
static const int TILE_SIZE = 800 / (TILECHIP_SIZE * SCALE);
static const int MENU_SIZE = 64;

inline static Sprite* CreateSprite(const std::string& filename, Rect rect)
{
	Sprite* sprite = Sprite::create(filename);
	sprite->setTextureRect(CC_RECT_PIXELS_TO_POINTS(rect));
	sprite->setContentSize(Size(rect.size.width, rect.size.height));
	return sprite;
}

inline static Sprite* SetTextureRect(Sprite* sprite, Rect rect)
{
	sprite->setTextureRect(CC_RECT_PIXELS_TO_POINTS(rect));
	sprite->setContentSize(Size(rect.size.width, rect.size.height));
	return sprite;
}

inline int GetFieldX(int index)
{
	return index%TILE_SIZE*TILECHIP_SIZE;
}

inline int GetFieldY(int index)
{
	return index/TILE_SIZE*TILECHIP_SIZE;
}

inline int GetFieldIndex(float x, float y, int dx=0, int dy=0)
{
	return ((int)(x)/(TILECHIP_SIZE*SCALE) + dx) + ((int)(y)/(TILECHIP_SIZE*SCALE) + dy)*TILE_SIZE;
}

void Field::Initialize(Node* scene)
{
	m_Scene = scene;
	m_Map.resize(TILE_SIZE * TILE_SIZE);
	m_Tiles.resize(TILE_SIZE * TILE_SIZE);
	
	m_Root = Sprite::create();
	m_Root->setPosition(0, 0);
	m_Root->setScale(SCALE);
	
	m_Chips.push_back(7);
	m_Chips.push_back(241);
	m_Chips.push_back(3);
	m_Chips.push_back(6);
	m_Chips.push_back(359);
	m_Chips.push_back(116);
	m_Chips.push_back(379);
	m_Menu.resize(m_Chips.size());
	
	for (int i=0; i<m_Map.size(); i++)
	{
		int x = GetFieldX(i);
		int y = GetFieldY(i);
		
		int cx = m_Chips[1] % 30;
		int cy = m_Chips[1] / 30;
		Rect rect = Rect(cx*TILECHIP_SIZE, cy*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
		
		Sprite* chip = CreateSprite("chip02d_dungeon.png", rect);
		chip->setAnchorPoint({0, 0});
		chip->setPosition(Vec2(x, y));
		
		m_Map[i] = 1;
		m_Tiles[i] = chip;
		
		m_Root->addChild(m_Tiles[i]);
	}
	m_Scene->addChild(m_Root);
	
	// menu
	for (int i=0; i<m_Menu.size(); i++)
	{
		int cx = m_Chips[i] % 30;
		int cy = m_Chips[i] / 30;
		Rect rect = Rect(cx*TILECHIP_SIZE, cy*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
		
		Sprite* chip = CreateSprite("chip02d_dungeon.png", rect);
		chip->setPosition(Vec2(810, MENU_SIZE*i));
		chip->setAnchorPoint({0, 0});
		chip->setScale(MENU_SIZE / TILECHIP_SIZE);
		
		m_Menu[i] = chip;
		m_Scene->addChild(m_Menu[i]);
	}
#if 1
	visual = cocos2d::TMXTiledMap::create("field.tmx");
	//	visual->setPosition({0, 32});
	m_Scene->addChild(visual);
#endif
}

int Field::GetMap(float x, float y) const
{
	int index = GetFieldIndex(x, y);
	if (index >= 0 && index < m_Map.size())
		return m_Map[index];
	
	return -1;
}

int Field::SelectTile(float x, float y)
{
	if (x < 800) return -1;
	
	int m = (int)(y) / (MENU_SIZE);
	
	if (m_Menu.size() >= m)
	{
		m_Tile = m;
		return m;
	}
	return -1;
}

int Field::Dig(float x, float y)
{
	if (x > 800) return 0;
	
	int index = GetFieldIndex(x, y);
	
#if 0
	cocos2d::TMXLayer* layer = visual->getLayer("base");
	auto tiles = layer->getTiles();
	
	tiles[index] = 16 * 5;
#endif
	
	if (m_Map[index] != m_Tile)
	{
		int cx = m_Chips[m_Tile] % 30;
		int cy = m_Chips[m_Tile] / 30;
		Rect rect = Rect(cx*TILECHIP_SIZE, cy*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
		SetTextureRect(m_Tiles[index], rect);
		m_Map[index] = m_Tile;
		
		if (m_OnCreateTile)
		{
			m_OnCreateTile(m_Chips[m_Tile], index, GetFieldX(index)*SCALE+TILECHIP_SIZE, GetFieldY(index)*SCALE+TILECHIP_SIZE);
		}
	}
	
	return m_Tile;
}
