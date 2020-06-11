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

		
		switch(boxComp.m_CollisionDir)
		{
		case eDirection::DOWN:
				transform.position.y += movPhysicsComp.m_Velocity.y * boxComp.m_EntryTime;
				transform.position.x += movPhysicsComp.m_Velocity.x;
			break;

		case eDirection::UP:
			transform.position.y += movPhysicsComp.m_Velocity.y * boxComp.m_EntryTime;
			transform.position.x += movPhysicsComp.m_Velocity.x;
		break;

		case eDirection::LEFT:
				transform.position.y += movPhysicsComp.m_Velocity.y;
				transform.position.x += movPhysicsComp.m_Velocity.x * boxComp.m_EntryTime;
			break;

		case eDirection::NONE:
			transform.position += movPhysicsComp.m_Velocity;
			break;
		}

	
		//transform.position.x += velocity.m_Velocity.x * dt;

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
			
		switch(boxComp.m_CollisionDir)
		{
		case eDirection::DOWN:
			if(movPhysicsComp.m_Force.y >= 0.f)
			{
				movPhysicsComp.m_Force.y = 0;
			}
			break;
		default:
			movPhysicsComp.m_Force -= gravity.m_Gravity * dt;
			break;
		}


		movPhysicsComp.m_Velocity = movPhysicsComp.m_Force * dt;
	}
}
