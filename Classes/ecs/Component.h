#pragma once

#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "EntityRegistry.h"
#include "Vector.h"

namespace ecs2
{
	class ComponentRegistry;
	
	struct ComponentHandle
	{
		int index;

		bool IsValid() const
		{
			return index != -1;
		}
	};

	class IUpdatable
	{
	public:
		virtual void Update(EntityRegistry& registry, float dt) = 0;
	};

	class IComponent
	{
	public:

		virtual void Assign() = 0;
		virtual ComponentHandle Attach(Entity entity) = 0;
		//virtual void Cleanup() = 0;
		virtual void GC(const EntityRegistry& registry) = 0;
	};
	
	class Component : public IComponent
	{
		friend ComponentRegistry;
		
		ComponentRegistry* m_ComponentRegistry;
		PackedArray<Entity> m_Entity;
		std::unordered_map<EntityId, int> m_LUTable;
		std::vector<Entity> m_AssignList;
		std::vector<Entity> m_RemoveList;

	protected:
		
		// entityを登録した時にcomponentを初期化する
		virtual void Reset(int index) = 0;
		
		// GCが実行されたときにcomponentを再配置する
		virtual void Compact(int index, int lastIndex) = 0;
		
		ComponentRegistry* ComponentRegistry()
		{
			return m_ComponentRegistry;
		}
		
	public:
		virtual ~Component()
		{}
		
		virtual void Initialize(EntityRegistry& registry, int maxSize)
		{
			m_Entity.resize(maxSize);
			m_LUTable.clear();
		}

		virtual void OnCreate(int index)
		{}
		
		int Size() const
		{
			return (int)m_Entity.size() - (int)m_AssignList.size();
		}
		
		inline Entity GetEntity(int index) const
		{
			return m_Entity[index];
		}
		
		ComponentHandle GetHandle(Entity entity) const
		{
			if (m_LUTable.count(entity.Id) == 0)
			{
				printf("index:%d gen:%d\n", entity.Index(), entity.Generation());
				assert(false);	// 存在しないentity
				return {-1};
			}
			
			ComponentHandle h = {m_LUTable.at(entity.Id)};
			return h;
		}
		
		bool HasComponent(Entity entity) const
		{
			return (m_LUTable.count(entity.Id) > 0);
		}
		
		ComponentHandle Attach(Entity entity) override
		{
			assert(m_LUTable.count(entity.Id) == 0);
			
			if (m_LUTable.count(entity.Id) == 0)
			{
				m_AssignList.push_back(entity);
				
				auto n = m_Entity.size();
				m_Entity.push_back(entity);
				m_LUTable[entity.Id] = n;
				Reset(n);
				
				ComponentHandle h = { n };
				return h;
			}
			else
			{
				ComponentHandle h = { m_LUTable[entity.Id] };
				return h;
			}
		}
		
		void Assign() override
		{
			if (!m_AssignList.empty())
			{
				for (Entity entity : m_AssignList)
				{
					assert(HasComponent(entity));
					
					OnCreate(m_LUTable.at(entity.Id));
				}
				m_AssignList.clear();
			}
		}
		
		void GC(const EntityRegistry& registry) override
		{
			int alive_in_row = 0;
			
			while (m_Entity.size() > 0 && alive_in_row < 4)
			{
				int n = mt() % m_Entity.size();
				if (registry.Alive(m_Entity[n]))
				{
					++alive_in_row;
					continue;
				}
				
				alive_in_row = 0;
				
				// entityが死んでいたらコンポーネントも削除する
				Remove(n);
			}
		}
		
	protected:
		// GCされたコンポーネントを削除します
		void Remove(int index)
		{
			int lastIndex = m_Entity.size() - 1;
			Entity entity = m_Entity[index];
			Entity lastEntity = m_Entity[lastIndex];
			
			// 削除したentityの位置に最後のコンポーネントをコピーする
			m_Entity.remove(index);
			Reset(index);
			Compact(index, lastIndex);
			
			m_LUTable[lastEntity.Id] = index;
			m_LUTable.erase(entity.Id);
		}
	};

}
