
#include "Physics.h"

void Physics::Initialize(int maxSize)
{
	m_Size = 0;
	Velocity.resize(maxSize);
	Acceleration.resize(maxSize);
	Friction.resize(maxSize);
}

void Physics::Update(float dt)
{
	for (int i=0; i<m_Size; i++)
	{
		Vector2f& velocity = Velocity[i];
		Vector2f& accelaration = Acceleration[i];
		Velocity[i] *= Friction[i];
		
		velocity += accelaration * dt;
	}
}

void Physics::AddRigidBody(RigidBody& body)
{
	int i = m_Size;
	Velocity[i] = body.Velocity;
	Acceleration[i] = body.Acceleration;
	Friction[i] = body.Friction;
	m_Size++;
}

void Physics::RemoveRigidBody(RigidBody& body)
{
}
