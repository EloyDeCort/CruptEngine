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

}

static void EntityUpdate(Entity entity)
{
	crupt::ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	//Get the transform & Renderable component
	TransformComponent& transform = coordinator->GetComponent<TransformComponent>(entity);
	MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
	BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
	
	transform.position.x += movPhysicsComp.m_Velocity.x * boxComp.m_EntryTimeX;
	
	movPhysicsComp.m_Force.x = 0;

	transform.position.y += movPhysicsComp.m_Velocity.y * boxComp.m_EntryTimeY;

	boxComp.m_CollisionRect.x = int(transform.position.x);
	boxComp.m_CollisionRect.y = int(transform.position.y);
}

void PhysicsSystem::Update(float dt)
{
	dt;
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
	

		switch(boxComp.m_ColDirY)
		{
		case eDirection::DOWN:
		case eDirection::UP:
			if(movPhysicsComp.m_Force.y >= 0.f)
			{
				movPhysicsComp.m_Force.y = 0;
			}
			break;
		default:
			break;
		}

		//Gravity
		movPhysicsComp.m_Force -= gravity.m_Gravity * dt;

		movPhysicsComp.m_Velocity = movPhysicsComp.m_Force * dt;
	}
}

