#pragma once
#include "ECSTypes.h"
#include <queue>
#include <array>
#include "Singleton.h"

namespace crupt
{
	class EntityManager : public Singleton<EntityManager>
	{
	public:
		EntityManager();
		~EntityManager() = default;

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, Signature signature);
		Signature GetSignature(Entity entity) const;

	private:
		friend class Singleton<EntityManager>;
		//Queue only supports insert back and pop front. Deque is not needed for the entities.
		//When an entity is created it takes an id from the front, when destroyed it puts the destroyed id at the end of the queue.
		std::queue<Entity> m_AvailableEntities;
		std::array<Signature, Max_Entities> m_Signatures;
		uint32_t m_NrOfLivingEntities;
	};
}

