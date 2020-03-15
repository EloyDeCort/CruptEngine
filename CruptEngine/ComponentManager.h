#pragma once
#include "ECSTypes.h"
#include <unordered_map>
#include "ComponentArray.h"

namespace crupt
{		
	class ComponentManager
	{
	public:
		~ComponentManager()
		{
			for (std::pair<const char*, ComponentArrayInterface*> pair : m_ComponentArrays)
			{
				delete pair.second;
				pair.second = nullptr;
			}
		}

		template<typename T>
		void RegisterComponent()
		{
			//Get the name of the component class and set this as the typename
			const char* typeName{typeid(T).name()};

			//Check if component already excists and if we're registering it more than once
			if(m_ComponentTypes.find(typeName) != m_ComponentTypes.end())
				throw std::exception("ComponentManager<T>::RegisterComponent - [ERROR] Component is already registered!");

			//Add component type to the map of types
			m_ComponentTypes.insert({typeName, m_NextComponentType});

			//Create new pointer to a component array and add it to the map
			m_ComponentArrays.insert({typeName,	new ComponentArray<T>()});

			//Increase next component to ensure we can have a multitude of components
			++m_NextComponentType;
		}
		template<typename T>
		ComponentType GetComponentType()
		{
			//Get the name of the component
			const char* typeName{typeid(T).name()};
	
			//Check if the component excists
			if(m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
				throw std::exception("ComponentManager<T>::GetComponentType - [ERROR] Component could not be found! (Did you register it?)");

			//return the type of the component, this will be used to create the signatures.
			return m_ComponentTypes[typeName];
		}
		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			//Add a component to the array for said entity
			GetComponentArray()->InsertData(entity, component);
		}
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			//Remove a component from the array of an entity
			GetComponentArray()->RemoveData(entity);
		}
		template<typename T>
		T& GetComponent(Entity entity)
		{
			//Return a reference to the component from the array of components for the entity
			return GetComponentArray()->GetData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			//Go through all component arrays to notify them that an entity has been destroyed
			//If it has a component for the entity it should be removed.
			for(const std::pair<const char*, ComponentArrayInterface*>& pair : m_ComponentArrays )
			{
				ComponentArrayInterface* component = pair.second;
				component->EntityDestroyed(entity);
			}
		}

	private:
		//returns a pointer to the component array
		template<typename T>
		ComponentArray<T>* GetComponentArray()
		{
			//Get the name of the component
			const char* typeName{typeid(T).name()};

			if(m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
				throw std::exception("ComponentManager<T>::GetComponentArray - [ERROR] Component could not be found! (Did you register it?)");
	
			return m_ComponentArrays[typeName];		
		}

		//Map that Stores char pointer (string) to component type, this will just map all new added components with a name to the component type ID.
		std::unordered_map<const char*, ComponentType> m_ComponentTypes;

		//Map that stores char pointers (string/name) to a component array, will include every new added component
		std::unordered_map<const char*, ComponentArrayInterface*> m_ComponentArrays;

		//Will be used as an indicator of how many different components there are and what the next ID if one is added.
		ComponentType m_NextComponentType;
	};
}

