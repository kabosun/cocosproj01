#pragma once
#include "newecs.h"

class NewECSSample
{
	ecs::EntityManager manager;
	
public:
	void Init();
	
	void Update(float delta);
};
