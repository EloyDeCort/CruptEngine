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
		VelocityComponent& velocity = m_pCoordinator->GetComponent<VelocityComponent>(entity);
		BoxCollisionComponent& boxComp = m_pCoordinator->GetComponent<BoxCollisionComponent>(entity);

		
		transform.position += velocity.m_Velocity * boxComp.m_EntryTime;
	
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
		VelocityComponent& velocity = m_pCoordinator->GetComponent<VelocityComponent>(entity);
		BoxCollisionComponent& boxComp = m_pCoordinator->GetComponent<BoxCollisionComponent>(entity);
			
	
		if(!boxComp.m_IsGrounded)
		{
			//Basic gravity
			velocity.m_Force -= gravity.m_Gravity * dt;
		}
		else
		{
			if(velocity.m_Force.y >= 0.f)
			{
				velocity.m_Force.y = 0;
			}
		}

		velocity.m_Velocity = velocity.m_Force * dt;
	}
}
