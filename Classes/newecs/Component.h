#pragma once
#include "../ecs/Vector.h"

namespace ecs
{
	struct Position : public Vector2f
	{
		using Vector2f::Vector2f;
	};
	
	struct Sprite
	{
		int Id;
		
		Sprite(int id)
		{
			Id = id;
		}
	};
}
