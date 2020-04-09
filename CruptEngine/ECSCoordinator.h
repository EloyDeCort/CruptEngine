#pragma once
#include "ECSTypes.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "Singleton.h"


//USED AS WRAPPED FOR ALL OUR ECS MANAGERS
namespace crupt
{
	class ECSCoordinator : public Singleton<ECSCoordinator>
	{
	public:
		void Initialize()
		{
			ComponentManager::GetInstance();
			EntityManager::GetInstance();
			SystemManager::GetInstance();
		}

		//Entities
		Entity CreateEntity()
		{
			return EntityManager::GetInstance().CreateEntity();
		}
		void DestroyEntity(Entity entity)
		{
			//Destroy and let the content manager and system manager know the entity is gone so they can update their maps
			EntityManager::GetInstance().DestroyEntity(entity);
			ComponentManager::GetInstance().EntityDestroyed(entity);
			SystemManager::GetInstance().EntityDestroyed(entity);		
		}

		//Components
		template<typename T>
		void RegisterComponent()
		{
			ComponentManager::GetInstance().RegisterComponent<T>();
		};

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			//Add the component to the manager
			ComponentManager::GetInstance().AddComponent(entity, component);
			//get reference to the signature of the entity
			Signature signature{EntityManager::GetInstance().GetSignature(entity)};
			//set the signature of the entity to include the new component
			signature.set(ComponentManager::GetInstance().GetComponentType<T>(), true);
			EntityManager::GetInstance().SetSignature(entity, signature);

			SystemManager::GetInstance().EntitySignatureChanged(entity, signature);
		}	

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			ComponentManager::GetInstance().RemoveComponent<T>(entity);

			Signature signature{EntityManager::GetInstance().GetSignature(entity)};
			//remove the component from the signature
			signature.set(ComponentManager::GetInstance().GetComponent<T>(), false);
			EntityManager::GetInstance().SetSignature(entity, signature);

			SystemManager::GetInstance().EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return ComponentManager::GetInstance().GetComponent<T>(entity);		
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return ComponentManager::GetInstance().GetComponentType<T>();
		}

		//Systems
		template<typename T>
		T* RegisterSystem()
		{
			return SystemManager::GetInstance().RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			SystemManager::GetInstance().SetSignature<T>(signature);
		}

		template<typename T>
		T* GetSystem()
		{
			return SystemManager::GetInstance().GetSystem<T>();
		}

	private:
	
	};
}

