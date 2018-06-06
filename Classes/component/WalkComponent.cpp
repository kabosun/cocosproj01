#include "WalkComponent.h"
#include "TransformComponent.h"
#include "../ecs/ComponentRegistry.h"

USING_NS_CC;

int GetFieldIndex(float x, float y, int dx=0, int dy=0);

void WalkComponent::Update(EntityRegistry& registry, float dt)
{
	static const int Width = 12;
	static const int Height = 12;
	
	assert(Transform);
	assert(field);
	bool eval = false;
	if (m_ReevaluateFrame == 0)
	{
		auto map = field->GetMap();
		for (int i=0; i<map.size(); i++)
		{
			int t = map[i];
			if (t == 0 && m_Evaluation[i] < 0.1f)
				m_Evaluation[i] = 1;	// 壁がない
			
			
//			auto a = std::to_string((int)(m_History[i]));
//			m_Sprites[i]->setString(a);
		}
		m_ReevaluateFrame = 60;
		eval = true;
		
		printf("eval\n");
	}
	m_ReevaluateFrame--;
	
	for (int i=0; i<Size(); i++)
	{
		auto handle = Transform->GetHandle(GetEntity(i));
		auto position = Transform->GetPosition(handle);
		
		// 評価の高い方向へ進む
		
		if (eval)
		{
			int direction = 0;
			int leftIndex = GetFieldIndex(position.X, position.Y, -1, 0);
			int rightIndex = GetFieldIndex(position.X, position.Y, 1, 0);
			int upIndex = GetFieldIndex(position.X, position.Y, 0, 1);
			int downIndex = GetFieldIndex(position.X, position.Y, 0, -1);
			
			float left = m_Evaluation[leftIndex];
			float right = m_Evaluation[rightIndex];
			float up = m_Evaluation[upIndex];
			float down = m_Evaluation[downIndex];
			if (left > 0 && left >= right && left >= up && left >= down)
			{
				direction = 2;
			}
			else if (right > 0 && right >= left && right >= up && right >= down)
			{
				direction = 0;
			}
			else if (up > 0 && up >= right && up >= left && up >= down)
			{
				direction = 3;
			}
			else if (down > 0 && down >= right && down >= up && down >= left)
			{
				direction = 1;
			}
			
			m_Data.Direction[i] = direction;
		}
		
		int direction = 0;//m_Data.Direction[i];
		
		int vx = 0;
		int vy = 0;
		switch(direction)
		{
			case 0:	// right
			{
				vx = 1;
				float dx = m_Data.Speed[i] * dt * vx;
				float dy = m_Data.Speed[i] * dt * vy;
				
				float x = position.X+Width+dx;
				float y = position.Y+Height-1;
				float y2 = position.Y-Height;
				
				if (field->GetMap(x, y) == 0 && field->GetMap(x, y2) == 0)
				{
					position.X += dx;
					position.Y += dy;
					Transform->SetPosition(handle, position);
					
					int index = GetFieldIndex(position.X, position.Y);
					m_History[index] = 1;
					return;
				}
				break;
			}
			case 1:	// down
			{
				vy = -1;
				float dx = m_Data.Speed[i] * dt * vx;
				float dy = m_Data.Speed[i] * dt * vy;
				
				float x = position.X-Width;
				float x2 = position.X+Width-1;
				float y = position.Y-Height+dy;
				
				if (field->GetMap(x, y) == 0 && field->GetMap(x2, y) == 0)
				{
					position.X += dx;
					position.Y += dy;
					Transform->SetPosition(handle, position);
					
					int index = GetFieldIndex(position.X, position.Y);
					m_History[index] = 1;
					return;
				}
				break;
			}
			case 2:	// left
			{
				vx = -1;
				float dx = m_Data.Speed[i] * dt * vx;
				float dy = m_Data.Speed[i] * dt * vy;
				
				float x = position.X-Width+dx;
				float y = position.Y+Height-1;
				float y2 = position.Y-Height;
				
				if (field->GetMap(x, y) == 0 && field->GetMap(x, y2) == 0)
				{
					position.X += dx;
					position.Y += dy;
					Transform->SetPosition(handle, position);
					
					int index = GetFieldIndex(position.X, position.Y);
					m_History[index] = 1;
					return;
				}
				break;
			}
			case 3:	// up
			{
				vy = 1;
				float dx = m_Data.Speed[i] * dt * vx;
				float dy = m_Data.Speed[i] * dt * vy;
				
				float x = position.X-Width;
				float x2 = position.X+Width-1;
				float y = position.Y+Height+dy;
				
				if (field->GetMap(x, y) == 0 && field->GetMap(x2, y) == 0)
				{
					position.X += dx;
					position.Y += dy;
					Transform->SetPosition(handle, position);
					
					int index = GetFieldIndex(position.X, position.Y);
					m_History[index] = 1;
					return;
				}
				break;
			}
		}
	}
}

