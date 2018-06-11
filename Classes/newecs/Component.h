#pragma once
#include "../ecs/Vector.h"

namespace ecs
{
	enum class ComponentId
	{
		None,
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
