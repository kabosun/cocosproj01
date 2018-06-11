#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../ecs/Entity.h"
#include "../ecs/Vector.h"

using namespace game;
using namespace ecs2;

class TransformComponent;

struct Shape
{
	int shapeType = 0;
};

struct RectShape : public Shape
{
	Rect rect;
	
	RectShape()
	{}
	
	RectShape(float x, float y, float width, float height)
	{
		this->shapeType = 1;
		rect.origin.x = x;
		rect.origin.y = y;
		rect.size.width = width;
		rect.size.height = height;
	}
};

struct TileShape : public Shape
{
	std::vector<int> Tiles;
	
	TileShape()
	{}
	
	TileShape(std::vector<int>& tiles)
	{
		this->shapeType = 100;
		Tiles = tiles;
	}
};

struct RigidBody
{
	Entity entity;
	int Group;
	Vector2f Velocity;
	Vector2f Acceleration;
	float Friction;
	std::shared_ptr<RectShape> shape;
};

class Physics
{
	using FnCollide = std::function<void(Entity, Entity)>;
	FnCollide m_OnCollide = nullptr;
	
	float PrevDelta;
	TransformComponent* Transform;
	
	PackedArray<RigidBody> RigidBodys;
	std::unordered_map<EntityId, int> RigidBodysLUT;
	PackedArray<Point> PrevRigidBodyPositions;	// 1つ前の位置
#if 0
	PackedArray<Entity> Entities;
	PackedArray<RectShape> RectShapes;
	PackedArray<TileShape> TileShapes;
	std::unordered_map<EntityId, int> RectShapesLUT;
	std::unordered_map<EntityId, int> TileShapesLUT;
	std::unordered_map<EntityId, int> ShapeTypeLUT;
#endif
	std::unordered_map<int, std::tuple<int, int, Size>> PreColliders;
	
public:
	void Initialize(int maxSize=1024);
	
	void Update(float dt);
	
	RigidBody& AddBody(RigidBody& body);
	
	void RemoveBody(Entity entity);
	
#if 0
	void AddShape(RectShape& shape);
	
	void AddShape(TileShape& shape);
	
	void RemoveShape(Entity entity);
#endif
	
	void SetSharedComponent(TransformComponent* transform)
	{
		this->Transform = transform;
	}
	
	void SetOnCollideFunc(FnCollide func)
	{
		m_OnCollide = func;
	}
};
