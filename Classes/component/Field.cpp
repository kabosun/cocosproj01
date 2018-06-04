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
	m_Menu.resize(4);
	
	m_Root = Sprite::create();
	m_Root->setPosition(0, 0);
	m_Root->setScale(SCALE);
	
	for (int i=0; i<m_Map.size(); i++)
	{
		int x = GetFieldX(i);
		int y = GetFieldY(i);
		Sprite* chip = CreateSprite("chip02d_dungeon.png", Rect(0*TILECHIP_SIZE, 0*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE));
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
		Sprite* chip = CreateSprite("chip02d_dungeon.png", Rect(0*TILECHIP_SIZE, i*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE));
		chip->setPosition(Vec2(810, MENU_SIZE*(i+1)));
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

void Field::Dig(float x, float y)
{
	if (x > 800) return;
	
	int index = GetFieldIndex(x, y);
	
#if 0
	cocos2d::TMXLayer* layer = visual->getLayer("base");
	auto tiles = layer->getTiles();
	
	tiles[index] = 16 * 5;
#endif
	
	if (m_Map[index] != m_Tile)
	{
		m_Map[index] = 0;
		//m_Root->removeChild(m_Tiles[index]);
		
		switch (m_Tile)
		{
			case 0:
			{
				Rect rect = Rect((8-1)*TILECHIP_SIZE, (0)*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
				SetTextureRect(m_Tiles[index], rect);
				break;
			}
			default:
			{
				m_Map[index] = m_Tile;
				Rect rect = Rect(0*TILECHIP_SIZE, (m_Tile-1)*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
				SetTextureRect(m_Tiles[index], rect);
				break;
			}
		}
	}
}
