#pragma once

#include <vector>
#include "../ecs/Vector.h"

class Input
{
	int m_KeyState = 0;
	std::vector<Vector2f> m_Location;
	
public:
	int GetKeyState() const
	{
		return m_KeyState;
	}
	
	std::vector<Vector2f> GetLocation() const
	{
		return m_Location;
	}
	
	void Reset()
	{
		m_KeyState = 0;
		m_Location.clear();
	}
	
	void KeyPress(int key);
	
	void KeyRelease(int key);
	
	void TouchBegin(Vector2f location);
	
	void TouchMove(Vector2f location);
	
	void TouchEnd(Vector2f location);
};
