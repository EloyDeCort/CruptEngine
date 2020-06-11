#include "CruptEnginePCH.h"
#include "PhysicsSystem.h"
#include "Components.h"
using namespace crupt;

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Init()
{
	m_pCoordinator = &ECSCoordinator::GetInstance();
}

void PhysicsSystem::Update(float dt)
{
	dt;
	for (const Entity& entity : m_Entities)
	{	
		//Get the transform & Renderable component
		TransformComponent& transform = m_pCoordinator->GetComponent<TransformComponent>(entity);
		//const GravityComponent& gravity = m_pCoordinator->GetComponent<GravityComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = m_pCoordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = m_pCoordinator->GetComponent<BoxCollisionComponent>(entity);
	
		transform.position.x += movPhysicsComp.m_Velocity.x * boxComp.m_EntryTimeX;
		transform.position.y += movPhysicsComp.m_Velocity.y * boxComp.m_EntryTimeY;

		boxComp.m_CollisionRect.x = int(transform.position.x);
		boxComp.m_CollisionRect.y = int(transform.position.y);
		
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
