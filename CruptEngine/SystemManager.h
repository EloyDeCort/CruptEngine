#pragma once
#include <unordered_map>
#include "ECSTypes.h"
#include "System.h"
#include "Singleton.h"

namespace crupt
{
	class SystemManager : public Singleton<SystemManager>
	{
	public:
		~SystemManager()
		{
			////Remove all the system objects
			for (std::pair<const char*, System*> pair : m_Systems)
			{
				delete pair.second;
				pair.second = nullptr;
			}
		}
		template<typename T>
		T* RegisterSystem()
		{
			//Get the name of the system that has to be registered
			const char* typeName{typeid(T).name()};

			//Check if system is already registered
			if(m_Systems.find(typeName) != m_Systems.end())
				throw std::exception("SystemManager<T>::RegisterSystem() - [ERROR] System is already registered!");

			//Create a new system of the given type, add this pointer to the system map
			T* system{new T()};
			m_Systems.insert({typeName, system});
			//return the pointer to use
			return system;
		}
		template<typename T>
		void SetSignature(Signature signature)
		{
			//Get the name of the system
			const char* typeName{typeid(T).name()};

			//Check if system excists/is registered
			if(m_Systems.find(typeName) == m_Systems.end())
				throw std::exception("SystemManager<T>::SetSignature - [ERROR] System could not be found! (Did you register it?)");

			//Set signature for the system
			m_Signatures.insert({typeName, signature});
		}

		template<typename T>
		T* GetSystem()
		{
			//Get the name of the system
			const char* typeName{typeid(T).name()};

			std::unordered_map<const char*, System*>::iterator it = m_Systems.find(typeName);
			//Check if system excists/is registered
			if(it == m_Systems.end())
				throw std::exception("SystemManager<T>::GetSystem - [ERROR] System could not be found! (Did you register it?)");

			T* system = reinterpret_cast<T*>((*it).second);
			//Return the system
			return system;
		}

		void EntityDestroyed(Entity entity)
		{
			//remove a given entity from all the systems.
			for (std::pair<const char*, System*> pair : m_Systems)
			{
				System* system{pair.second};
				system->m_Entities.erase(entity);
			}
		}
		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			//We have to notify	every system that the signature of the entity has changed.
			for (std::pair<const char*, System*> pair : m_Systems)
			{
				//get the needed data
				const char* type{pair.first};
				System* system{pair.second};
				Signature& systemSignature{m_Signatures[type]};

				//Check if the entity signature matches the system signature
				if((entitySignature & systemSignature) == systemSignature)
				{
					//If thats the case, add it to the set
					system->m_Entities.insert(entity);
				}
				else
				{
					//Entity signature does match the system signature, erase it from the entities set
					system->m_Entities.erase(entity);
				}
			}
		}
		SystemManager() = default;
	private:

		//Map from a system/name string to a signature
		std::unordered_map<const char*, Signature> m_Signatures;

		//Map from system string/name to a system pointer
		std::unordered_map<const char*, System*> m_Systems;
	};
}

