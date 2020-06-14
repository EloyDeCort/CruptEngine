#include "CruptEnginePCH.h"
#include "PhysicsSystem.h"
#include "Components.h"
#include <thread>
#include <future>
using namespace crupt;

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Init()
{
	m_pCoordinator = &ECSCoordinator::GetInstance();
	m_TerminalVelocity = 350.f;
}

void EntityUpdate(Entity entity)
{
	crupt::ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	//Get the transform & Renderable component
	TransformComponent& transform = coordinator->GetComponent<TransformComponent>(entity);
	MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
	BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
	
	//Apply the velocity and reset the force
	transform.position.x += movPhysicsComp.velocity.x * boxComp.entryTimeX;
	
	movPhysicsComp.force.x = 0;

	transform.position.y += movPhysicsComp.velocity.y * boxComp.entryTimeY;

	boxComp.collisionRect.x = int(transform.position.x);
	boxComp.collisionRect.y = int(transform.position.y);
}

void PhysicsSystem::Update(float dt)
{
	//Multithreading the physics system.
	std::vector<std::future<void>> futures;

	for (const Entity& entity : m_Entities)
	{	
		futures.push_back(std::async(std::launch::async, EntityUpdate, entity));
	}
}

void crupt::PhysicsSystem::PreUpdate(float dt)
{
	for (const Entity& entity : m_Entities)
	{	
		//Get the transform & Renderable component
		const GravityComponent& gravity = m_pCoordinator->GetComponent<GravityComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = m_pCoordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = m_pCoordinator->GetComponent<BoxCollisionComponent>(entity);
	

		switch(boxComp.colDirY)
		{
		case eDirection::DOWN:
		case eDirection::UP:
			if(movPhysicsComp.force.y >= 0.f)
			{
				movPhysicsComp.force.y = 0;
			}
			break;
		default:
			break;
		}

		//Gravity appliance
		movPhysicsComp.force -= gravity.gravity * dt;

		//Aplying the force.
		movPhysicsComp.velocity = movPhysicsComp.force * dt;
		if(movPhysicsComp.velocity.y > m_TerminalVelocity)
		{
			//Making sure the velocity does not overwrite the terminal velocity.
			movPhysicsComp.velocity.y = m_TerminalVelocity;
		}
	}
}

