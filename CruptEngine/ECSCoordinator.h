#pragma once
//Source & Inspiration: https://austinmorlan.com/posts/entity_component_system

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
			//Initialize all our ecs managers
			ComponentManager::GetInstance();
			EntityManager::GetInstance();
			SystemManager::GetInstance();
		}

		void DestroyAllEntities()
		{
			//Destroys all entities
			for(size_t i{}; i < m_Entities.size(); ++i)
			{
				EntityManager::GetInstance().DestroyEntity(m_Entities[i]);
				ComponentManager::GetInstance().EntityDestroyed(m_Entities[i]);
				SystemManager::GetInstance().EntityDestroyed(m_Entities[i]);
			}
			m_Entities.clear();
		}

		//Entities
		Entity CreateEntity()
		{
			//Make a new entity
			Entity e = EntityManager::GetInstance().CreateEntity();
			m_Entities.push_back(e);
			return e;
		}
		void DestroyEntity(Entity entity)
		{
			//Destroy and let the content manager and system manager know the entity is gone so they can update their maps
			EntityManager::GetInstance().DestroyEntity(entity);
			ComponentManager::GetInstance().EntityDestroyed(entity);
			SystemManager::GetInstance().EntityDestroyed(entity);
			m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity), m_Entities.end());
		}

		//Components
		template<typename T>
		void RegisterComponent()
		{
			//Register a component in the component manager
			ComponentManager::GetInstance().RegisterComponent<T>();
		};

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			//get reference to the signature of the entity
			Signature signature{EntityManager::GetInstance().GetSignature(entity)};

			//Making sure that the bit is not set yet (else we have a duplicate component)
			if(signature.test(ComponentManager::GetInstance().GetComponentType<T>()))
			{
				return;
			}

			//Add the component to the manager
			ComponentManager::GetInstance().AddComponent(entity, component);
	
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
			signature.set(ComponentManager::GetInstance().GetComponentType<T>(), false);
			EntityManager::GetInstance().SetSignature(entity, signature);
				
			SystemManager::GetInstance().EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return ComponentManager::GetInstance().GetComponent<T>(entity);		
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			return ComponentManager::GetInstance().HasComponent<T>(entity);		
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
		Signature GetSystemSignature()
		{
			return SystemManager::GetInstance().GetSignature<T>();
		}

		template<typename T>
		T* GetSystem()
		{
			return SystemManager::GetInstance().GetSystem<T>();
		}

	private:
		std::vector<Entity> m_Entities;
	};
}

