#include "CruptEnginePCH.h"
#include "BubbleMovementSystem.h"
#include "Components.h"
#include "GameComponents.h"
using namespace crupt;

BubbleMovementSystem::~BubbleMovementSystem()
{
	
}

void BubbleMovementSystem::Init()
{

}

void BubbleMovementSystem::Update(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;

	for (Entity entity : m_Entities)
	{
		BubbleComponent& bubbleComp = coordinator->GetComponent<BubbleComponent>(entity);
		if(bubbleComp.shouldPop)
		{
			toDelete.push_back(entity);
			continue;
		}

		TransformComponent& transComp = coordinator->GetComponent<TransformComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);

		transComp.position.x += movPhysicsComp.m_Velocity.x * dt * boxComp.m_EntryTimeX;
		transComp.position.y += movPhysicsComp.m_Velocity.y * dt * boxComp.m_EntryTimeY;

		boxComp.m_CollisionRect.x = int(transComp.position.x);
		boxComp.m_CollisionRect.y = int(transComp.position.y);

		if(boxComp.m_EntryTimeY < 0.001f)
		{
			toDelete.push_back(entity);
			continue;
		}

	}

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}

void crupt::BubbleMovementSystem::PreUpdate(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	for (Entity entity : m_Entities)
	{
		BubbleComponent& bubbleComp = coordinator->GetComponent<BubbleComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);

		bubbleComp.totalTime += dt;


		if(bubbleComp.totalTime < bubbleComp.xSpeedTime)
		{
			movPhysicsComp.m_Velocity.x = sin(5.f * bubbleComp.totalTime) * 20.f + bubbleComp.xSpeed;

			if(bubbleComp.flipped)
			{
				movPhysicsComp.m_Velocity.x = -movPhysicsComp.m_Velocity.x;
			}
		}
		else
		{
			movPhysicsComp.m_Velocity.x = sin(5.f * bubbleComp.totalTime) * 20.f ;
			movPhysicsComp.m_Velocity.y = -50.f;
		}
	}
}
