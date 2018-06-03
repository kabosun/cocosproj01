#include "Input.h"

void Input::KeyPress(int key)
{
	m_KeyState |= (1 << key);
}

void Input::KeyRelease(int key)
{
	m_KeyState &= ~(1 << key);
}

void Input::TouchBegin(Vector2f location)
{
	m_KeyState |= (1 << 16);
	m_Location.clear();
	m_Location.push_back(location);
}

void Input::TouchMove(Vector2f location)
{
	m_KeyState |= (1 << 17);
	m_Location.push_back(location);
}

void Input::TouchEnd(Vector2f location)
{
	m_KeyState &= ~(1 << 16);
	m_KeyState &= ~(1 << 17);
	m_Location.clear();
	m_Location.push_back(location);
}
