#include "Physics.h"
#include "TransformComponent.h"

void Physics::Initialize(int maxSize)
{
	PrevDelta = 1;
	RigidBodys.resize(maxSize);
	PrevRigidBodyPositions.resize(maxSize);
	//RectShapes.resize(maxSize);
	//TileShapes.resize(maxSize);
}

void Physics::Update(float dt)
{
	assert(Transform);
	
	for (auto pair : RigidBodysLUT)
	{
		int index = pair.second;
		auto& shape = RigidBodys[index].shape;
		
		// Transformの座標を反映する
		auto handle = Transform->GetHandle(RigidBodys[index].entity);
		auto position = Transform->GetPosition(handle);
		shape->rect.origin.x = position.X;
		shape->rect.origin.y = position.Y;
		
		// 速度を更新する
		RigidBody& body = RigidBodys[index];
		body.Velocity.X += (shape->rect.origin.x - PrevRigidBodyPositions[index].x) / PrevDelta;
		body.Velocity.Y += (shape->rect.origin.y - PrevRigidBodyPositions[index].y) / PrevDelta;
		body.Velocity *= 0.3;
		
#if 0
		if (RigidBodys[index].Group == 1)
		{
			if (shape->rect.origin.x - PrevRigidBodyPositions[index].x != 0)
			{
				printf("%d) X: %f <= %f\n", index, shape->rect.origin.x, PrevRigidBodyPositions[index].x);
			}
			if (shape->rect.origin.y - PrevRigidBodyPositions[index].y != 0)
			{
				printf("%d) Y: %f <= %f\n", index, shape->rect.origin.y, PrevRigidBodyPositions[index].y);
			}
			
			if (body.Velocity.X != 0)
			{
				printf("%d) VX: %f", index, body.Velocity.X);
			}
			if (body.Velocity.Y != 0)
			{
				printf("%d) VY: %f", index, body.Velocity.Y);
			}
		}
#endif
		
		// 位置を更新する
		shape->rect.origin.x += body.Velocity.X * dt;
		shape->rect.origin.y += body.Velocity.Y * dt;
		Transform->SetPosition(handle, Vector2f(shape->rect.origin.x, shape->rect.origin.y));
		
		PrevRigidBodyPositions[index] = shape->rect.origin;
	}
	
	// 衝突検知
	std::unordered_map<int, std::tuple<int, int, Size>> colliders;
	
	for (int i=0; i<RigidBodys.size()-1; i++)
	{
		for (int j=i+1; j<RigidBodys.size(); j++)
		{
			if (RigidBodys[i].Group == RigidBodys[j].Group) continue;

			auto& rect1 = RigidBodys[i].shape->rect;
			auto& rect2 = RigidBodys[j].shape->rect;
			
			int width = rect1.size.width / 2 + rect2.size.width / 2;
			float dx = (abs(rect1.origin.x - rect2.origin.x) - width);
			if (dx >= 0)
			{
				continue;
			}
			
			int height = rect1.size.height / 2 + rect2.size.height / 2;
			float dy = (abs(rect1.origin.y - rect2.origin.y) - height);
			if (dy >= 0)
			{
				continue;
			}
			
			// 衝突している
			colliders[(i+1) * (j+1)] = std::make_tuple(i, j, Size(dx, dy));
			
#if 1
			printf("%d) DX:%f DY:%f\n", i, dx, dy);
#endif
		}
	}
	
	if (m_OnCollide)
	{
		// enter
		for (auto pair : colliders)
		{
#if 1
			// 重なりを解消する
			int a[] = {std::get<0>(pair.second), std::get<1>(pair.second)};
			const Size& size = std::get<2>(pair.second);
			for (int n : a)
			{
				Entity entity = RigidBodys[n].entity;
				auto handle = Transform->GetHandle(entity);
				auto position = Transform->GetPosition(handle);
				
				Vector2f velocity = RigidBodys[n].Velocity.Normal();
				if (velocity.X == 0 && velocity.Y == 0)
				{
					continue;
				}
				position.X += size.width * velocity.X;
				position.Y += size.height * velocity.Y;
				Transform->SetPosition(handle, position);
				
				printf("ADJUST %d) X:%f Y:%f\n", n, position.X, position.Y);
			}
#endif
			int i = std::get<0>(pair.second);
			int j = std::get<1>(pair.second);

			// 衝突情報がなければ通知する
			if (PreColliders.count((i+1) * (j+1)) == 0)
			{
				Entity entity1 = RigidBodys[i].entity;
				Entity entity2 = RigidBodys[j].entity;
				
				m_OnCollide(entity1, entity2);
				m_OnCollide(entity2, entity1);
			}
		}
	}
	
	PreColliders = colliders;
	PrevDelta = dt;
}

RigidBody& Physics::AddBody(RigidBody& body)
{
	RigidBodys.push_back(body);
	PrevRigidBodyPositions.push_back(body.shape->rect.origin);
	
	RigidBodysLUT[body.entity.Id] = RigidBodys.size()-1;
	
	return RigidBodys[RigidBodys.size()-1];
}

void Physics::RemoveBody(Entity entity)
{
	int lastIndex = RigidBodys.size() - 1;
	Entity lastEntity = RigidBodys[lastIndex].entity;
	
	int index = RigidBodysLUT[entity.Id];
	RigidBodys.remove(index);
	PrevRigidBodyPositions.remove(index);
	
	RigidBodysLUT[lastEntity.Id] = RigidBodysLUT[entity.Id];
	RigidBodysLUT.erase(entity.Id);
}

#if 0
void Physics::AddShape(RectShape& shape)
{
	RectShapes.push_back(shape);
	
	RectShapesLUT[shape.entity.Id] = RectShapes.size()-1;
	ShapeTypeLUT[shape.entity.Id] = 1;
}

void Physics::AddShape(TileShape& shape)
{
	TileShapes.push_back(shape);

	TileShapesLUT[shape.entity.Id] = TileShapes.size()-1;
	ShapeTypeLUT[shape.entity.Id] = 2;
}

void Physics::RemoveShape(Entity entity)
{
	int type = ShapeTypeLUT.at(entity.Id);
	switch(type)
	{
		case 1:
		{
			int lastIndex = RectShapes.size() - 1;
			Entity lastEntity = RectShapes[lastIndex].entity;
			
			int index = RectShapesLUT[entity.Id];
			RectShapes.remove(index);
			
			RectShapesLUT[lastEntity.Id] = RectShapesLUT[entity.Id];
			RectShapesLUT.erase(entity.Id);
			ShapeTypeLUT.erase(entity.Id);
			break;
		}
		case 2:
		{
			int lastIndex = TileShapes.size() - 1;
			Entity lastEntity = TileShapes[lastIndex].entity;
			
			int index = TileShapesLUT[entity.Id];
			TileShapes.remove(index);
			
			TileShapesLUT[lastEntity.Id] = TileShapesLUT[entity.Id];
			TileShapesLUT.erase(entity.Id);
			ShapeTypeLUT.erase(entity.Id);
			break;
		}
	}
}
#endif
