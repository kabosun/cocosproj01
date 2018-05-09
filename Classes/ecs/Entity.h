#pragma once

#include <random>
#include <vector>
#include <typeindex>
#include "Vector.h"

namespace ecs2
{
	using EntityId = unsigned int;
	
	using Archetype = std::vector<std::type_index>;
	
	const unsigned ENTITY_INDEX_BITS = 22;
	const unsigned ENTITY_INDEX_MASK = (1<<ENTITY_INDEX_BITS)-1;

	const unsigned ENTITY_GENERATION_BITS = 8;
	const unsigned ENTITY_GENERATION_MASK = (1<<ENTITY_GENERATION_BITS)-1;

	static std::random_device rand;
	static std::mt19937 mt;
	
	struct Entity
	{
		EntityId Id;
		
		static Entity Make(unsigned int index, unsigned char generation)
		{
			Entity entity = {(generation << ENTITY_INDEX_BITS) | index};
			return entity;
		}
		
		unsigned int Index() const
		{
			return Id & ENTITY_INDEX_MASK;
		}
		
		unsigned int Generation() const
		{
			return (Id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK;
		}
		
		bool operator==(const Entity& e) const
		{
			return Id == e.Id;
		}
		
		bool operator!=(const Entity& e) const
		{
			return Id != e.Id;
		}
	};

	template<typename T>
	class PackedArray
	{
		size_t m_Size = 0;
		std::vector<T> m_Data;

	public:
		void resize(size_t size)
		{
			m_Data.resize(size);
		}

		void push_back(T& t)
		{
			m_Data[m_Size] = t;
			m_Size++;
		}

		void remove(int index)
		{
			int lastIndex = m_Size - 1;
			m_Data[index] = m_Data[lastIndex];

			--m_Size;
		}

		int size() const
		{
			return m_Size;
		}

		const T& operator[](int i) const
		{
			return m_Data[i];
		}

		T& operator[](int i)
		{
			return m_Data[i];
		}
	};
}
