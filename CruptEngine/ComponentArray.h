#pragma once
#include "ECSTypes.h"
#include <array>
#include <unordered_map>

namespace crupt
{
	//We use an interface to ensure that we can notify any type of component array that an entity is removed
	//This ComponentArray should then update its maps/arrays to ensure packed memory (which is the purpose and strength of an ECS system.
	//We also need this inheritance because we have a component array per component type, and all of these should know that the entity is destroyed,
	//To ensure that this entity is removed from their maps.

	//For now it is fine since entitydestroyed is not called every frame thus should have minimal impact.
	class ComponentArrayInterface
	{
	public:
		virtual void EntityDestroyed(Entity entity) = 0;
		virtual ~ComponentArrayInterface() = default;
	};

	//Templated class to support all types of components
	template<typename T>
	class ComponentArray : public ComponentArrayInterface
	{
	public:
		void InsertData(Entity entity, T component)
		{
			//Check if the component already excists on the current entity
			if(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
				throw std::exception("ComponentArray<T>::InsertData - [ERROR] Component added twice to same entity!");

			//Put new entry 
			size_t newIndex{m_Size};
			//Sets the correct connection from Entity to component index
			m_EntityToIndexMap[entity] = newIndex;
			//Sets the correct connection from the component index to the entity
			m_IndexToEntityMap[newIndex] = entity;
			//Add the component to the component array of the Entity
			m_ComponentArray[newIndex] = component;
			//Increase the size of the array to ensure that we can add multiple components
			++m_Size;
		}
		void RemoveData(Entity entity)
		{
			//Check if the entity excists
			if(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() )
				throw std::exception("ComponentArray<T>::RemoveData - [ERROR] Removed Entity does not have this component!");

			//Get the array index of the removed entity
			size_t indexRemovedEntity{m_EntityToIndexMap[entity]};
			//Get the index of the last element in the array
			size_t indexLastElement{m_Size - 1};
			//Put the component of the last element on the spot of the removed element.
			//This will ensure that the array is always dense and we have no holes in the array.
			m_ComponentArray[indexRemovedEntity] = m_ComponentArray[indexLastElement];

			//Get the entity ID of the last element in the array
			Entity entityLastElement{uint32_t(m_IndexToEntityMap[indexLastElement])};
			//Set the index of the last entity to the removed entity index
			m_EntityToIndexMap[entityLastElement] = indexRemovedEntity;
			//Set the entity of the removed entity to the entity of the last element
			m_IndexToEntityMap[indexRemovedEntity] = size_t(entityLastElement);

			//remove the entity from the entity map & remove the index from the index map
			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(indexLastElement);

			//Reduce the size of the array
			--m_Size;
		}
		T& GetData(Entity entity)
		{
			if(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end())
				throw std::exception("ComponentArray<T>::GetData - [ERROR] Trying to get a component that does not excist for this entity!");

			//Return a reference to the component of this entity
			//Get the index through the entity to index map (which will give us the correct index of what component)
			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}
		
		virtual void EntityDestroyed(Entity entity) override
		{
			if(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
			{
				//If the entity is registered in the map for this component, remove the data for this entity in this component map.
				RemoveData(entity);
			}
		}

	private:
		//Packed array of components, each entity should have a unique spot in the array.
		std::array<T, Max_Entities> m_ComponentArray;

		//Map Entity ID -> Array Index
		std::unordered_map<Entity, size_t> m_EntityToIndexMap;

		//Map Array Index -> Entity ID
		std::unordered_map<Entity, size_t> m_IndexToEntityMap;

		//amount of entries in the array
		size_t m_Size;


		//NOTE: We are using unordered maps which ensures we can do easy functions like find(), insert(),...
		//This can be used to easily validate data and make sure to have no duplicates.
	};


}