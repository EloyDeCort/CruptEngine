#pragma once
#include "ECSTypes.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"


//USED AS WRAPPED FOR ALL OUR ECS MANAGERS
namespace crupt
{
	class ECSCoordinator
	{
	public:
		ECSCoordinator() = default;
		~ECSCoordinator()
		{
			delete m_pComponentManager;
			delete m_pEntityManager;
			delete m_pSystemManager;
			m_pComponentManager = nullptr;
			m_pEntityManager = nullptr;
			m_pSystemManager = nullptr;
		}
		void Initialize()
		{
			//Initialize the managers
			m_pComponentManager = new ComponentManager();
			m_pEntityManager = new EntityManager();
			m_pSystemManager = new SystemManager();
		}

		//Entities
		Entity CreateEntity()
		{
			return m_pEntityManager->CreateEntity();
		}
		void DestroyEntity(Entity entity)
		{
			//Destroy and let the content manager and system manager know the entity is gone so they can update their maps
			m_pEntityManager->DestroyEntity(entity);
			m_pComponentManager->EntityDestroyed(entity);
			m_pSystemManager->EntityDestroyed(entity);		
		}

		//Components
		template<typename T>
		void RegisterComponent()
		{
			m_pComponentManager->RegisterComponent<T>();
		};

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			//Add the component to the manager
			m_pComponentManager->AddComponent(entity, component);
			//get reference to the signature of the entity
			Signature signature{m_pEntityManager->GetSignature(entity)};
			//set the signature of the entity to include the new component
			signature.set(m_pComponentManager->GetComponent<T>(), true);
			m_pEntityManager->SetSignature(entity, signature);

			m_pSystemManager->EntitySignatureChanged(entity, signature);
		}	

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			m_pComponentManager->RemoveComponent<T>(entity);

			Signature signature{m_pEntityManager->GetSignature(entity)};
			//remove the component from the signature
			signature.set(m_pComponentManager->GetComponent<T>(), false);
			m_pEntityManager->SetSignature(entity, signature);

			m_pSystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return m_pComponentManager->GetComponent<T>(entity);		
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return m_pComponentManager->GetComponentType<T>();
		}

		//Systems
		template<typename T>
		T* RegisterSystem()
		{
			return m_pSystemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			m_pSystemManager->SetSignature<T>(signature);
		}

	private:
		EntityManager* m_pEntityManager;
		ComponentManager* m_pComponentManager;
		SystemManager* m_pSystemManager;
	};
}

