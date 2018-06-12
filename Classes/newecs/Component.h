#pragma once
#include "../ecs/Vector.h"

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
		static const ComponentInfo Info() { return {static_cast<int>(ComponentId::Position), sizeof(Position)}; }
		
		using Vector2f::Vector2f;
	};
	
	struct Lifetime
	{
		static const ComponentInfo Info() { return {static_cast<int>(ComponentId::Lifetime), sizeof(Lifetime)}; }
		
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
		static const ComponentInfo Info() { return {static_cast<int>(ComponentId::Sprite), sizeof(Sprite)}; }
		
		int Id;
		
		Sprite(int id)
		{
			Id = id;
		}
	};
}
