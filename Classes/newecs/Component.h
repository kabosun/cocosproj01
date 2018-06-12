#pragma once
#include "../ecs/Vector.h"

#define COMPONENT_INFO(T)		static const ComponentInfo Info() { return {static_cast<int>(ComponentId::T), sizeof(T)}; }

namespace ecs
{
	enum class ComponentId
	{
		Position,
		Rotation,
		Velocity,
		Health,
		Lifetime,
		Sprite,
	};
	
	struct Position : public Vector2f
	{
		COMPONENT_INFO(Position);

		using Vector2f::Vector2f;
	};
	
	struct Lifetime
	{
		COMPONENT_INFO(Lifetime);
		
		int MaxValue;
		int Value;
		
		Lifetime(int max, int current)
		: MaxValue(max), Value(current)
		{}
		
		Lifetime(int max)
		: MaxValue(max), Value(max)
		{}
	};
	
	struct Sprite
	{
		COMPONENT_INFO(Sprite);
		
		int Id;
		
		Sprite(int id)
		: Id(id)
		{}
	};
}
