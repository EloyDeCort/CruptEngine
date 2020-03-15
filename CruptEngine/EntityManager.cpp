#include "CruptEnginePCH.h"
#include "EntityManager.h"

using namespace crupt;

crupt::EntityManager::EntityManager()
	: m_NrOfLivingEntities{}
{
	//Constructor Initializes queue with entity ids
	for(Entity e{}; e < Max_Entities; ++e)
	{
		m_AvailableEntities.push(e);
	}
}

Entity crupt::EntityManager::CreateEntity()
{
	//Making sure that we are not going above a certain amount of entities, determined by Max_Entitites
	if(m_NrOfLivingEntities >= Max_Entities)
		throw std::exception("EntityManager::CreateEntity - [ERROR] Maximum amount of Entities reached");

	//We take the first idea that was pushed (eg. 0) by taking the front of the queue.
	//We then remove it from the available entities to ensure no other entity can take the same ID
	Entity id = m_AvailableEntities.front();
	m_AvailableEntities.pop();
	//Increase number of entities that are currently living
	++m_NrOfLivingEntities;
	//returning the id of the acquired entity to use.
	return id;
}

void crupt::EntityManager::DestroyEntity(Entity entity)
{
	//Check if entity is not available due to the fatc that its bigger than the max allowed of entities.
	if(entity >= Max_Entities)
		throw std::exception("EntityManager::DestroyEntity - [ERROR] Entity is out of reach");
	
	//Reset the full signature of the given entity (clearing all components) (resets all bits to false)
	m_Signatures[entity].reset();

	//Add the new available ID to the end of the queue since now it can be used again.
	m_AvailableEntities.push(entity);

	//decrease number of living entities
	--m_NrOfLivingEntities;
}

void crupt::EntityManager::SetSignature(Entity entity, Signature signature)
{
	if(entity >= Max_Entities)
		throw std::exception("EntityManager::SetSignature - [ERROR] Entity is out of reach");

	//Add the signature to the array of signatures for this Entity.
	m_Signatures[entity] = signature;
}

Signature crupt::EntityManager::GetSignature(Entity entity) const
{
	if(entity >= Max_Entities)
		throw std::exception("EntityManager::SetSignature - [ERROR] Entity is out of reach");

	//returning the signature of the passed entity ID.
	return m_Signatures[entity];
}