#pragma once

#include <vector>
#include "../ecs/Vector.h"

struct RigidBody
{
	Vector2f Velocity;
	Vector2f Acceleration;
	float Friction;
};

class Physics
{
	int m_Size;
	std::vector<Vector2f> Velocity;
	std::vector<Vector2f> Acceleration;
	std::vector<float> Friction;
	
public:
	void Initialize(int maxSize);
	
	void Update(float dt);
	
	void AddRigidBody(RigidBody& body);
	
	void RemoveRigidBody(RigidBody& body);
};
