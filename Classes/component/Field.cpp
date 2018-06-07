#include "Field.h"

USING_NS_CC;

static const auto FILENAME = "tile.png";
static const int TILE_WIDTH = 128 / 16;
static const int TILECHIP_SIZE = 16;
static const int WIDTH = 400;
static const int TILE_SIZE = WIDTH / TILECHIP_SIZE;
static const int MENU_SIZE = 32;

inline static Sprite* CreateSprite(const std::string& filename, Rect rect)
{
	Sprite* sprite = Sprite::create(filename);
	sprite->setTextureRect(CC_RECT_PIXELS_TO_POINTS(rect));
	sprite->setContentSize(Size(rect.size.width, rect.size.height));
	sprite->setAnchorPoint({ 0, 0 });
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
	return ((int)(x)/TILECHIP_SIZE + dx) + ((int)(y)/TILECHIP_SIZE + dy)*TILE_SIZE;
}

void Field::Initialize(Node* scene)
{
	m_Scene = scene;
	m_Map.resize(TILE_SIZE * TILE_SIZE);
	m_Tiles.resize(TILE_SIZE * TILE_SIZE);
	
	m_Root = Sprite::create();
	m_Root->setPosition(0, 0);
	
	m_Chips.push_back(0);
	m_Chips.push_back(1);
	m_Chips.push_back(2);
	m_Chips.push_back(3);
	m_Chips.push_back(4);
	m_Chips.push_back(5);
	m_Chips.push_back(8);
	m_Menu.resize(m_Chips.size());
	
	for (int i=0; i<m_Map.size(); i++)
	{
		int x = GetFieldX(i);
		int y = GetFieldY(i);
		
		int cx = m_Chips[1] % TILE_WIDTH;
		int cy = m_Chips[1] / TILE_WIDTH;
		Rect rect = Rect(cx*TILECHIP_SIZE, cy*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
		
		Sprite* chip = CreateSprite(FILENAME, rect);
		chip->setPosition(Vec2(x, y));
		
		m_Map[i] = 1;
		m_Tiles[i] = chip;
		
		m_Root->addChild(m_Tiles[i]);
	}
	m_Scene->addChild(m_Root);
	
	// menu
	for (int i=0; i<m_Menu.size(); i++)
	{
		int cx = m_Chips[i] % TILE_WIDTH;
		int cy = m_Chips[i] / TILE_WIDTH;
		Rect rect = Rect(cx*TILECHIP_SIZE, cy*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
		
		Sprite* chip = CreateSprite(FILENAME, rect);
		chip->setPosition(Vec2(WIDTH+10, MENU_SIZE*i));
		chip->setScale(MENU_SIZE / TILECHIP_SIZE);
		
		m_Menu[i] = chip;
		m_Scene->addChild(m_Menu[i]);
	}
#if 0
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
	if (x < WIDTH) return -1;
	
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
	if (x > WIDTH) return 0;
	
	int index = GetFieldIndex(x, y);
	
#if 0
	cocos2d::TMXLayer* layer = visual->getLayer("base");
	auto tiles = layer->getTiles();
	
	tiles[index] = 16 * 5;
#endif
	
	if (m_Map[index] != m_Tile)
	{
		int cx = m_Chips[m_Tile] % TILE_WIDTH;
		int cy = m_Chips[m_Tile] / TILE_WIDTH;
		m_Map[index] = m_Tile;

		switch (m_Chips[m_Tile])
		{
			case 5:
				break;
			default:
			{
				Rect rect = Rect(cx*TILECHIP_SIZE, cy*TILECHIP_SIZE, TILECHIP_SIZE, TILECHIP_SIZE);
				SetTextureRect(m_Tiles[index], rect);
			}
		}
		
		if (m_OnCreateTile)
		{
			m_OnCreateTile(m_Chips[m_Tile], index, GetFieldX(index)+TILECHIP_SIZE, GetFieldY(index)+TILECHIP_SIZE);
		}
	}
	
	return m_Tile;
}
