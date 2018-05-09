#pragma once

#include <vector>
#include "../ecs/Vector.h"

struct RigidBody
{
	Vector3f Velocity;
	Vector3f Acceleration;
	float Friction;
};

class Physics
{
	int m_Size;
	std::vector<Vector3f> Velocity;
	std::vector<Vector3f> Acceleration;
	std::vector<float> Friction;
	
public:
	void Initialize(int maxSize);
	
	void Update(float dt);
	
	void AddRigidBody(RigidBody& body);
	
	void RemoveRigidBody(RigidBody& body);
};
