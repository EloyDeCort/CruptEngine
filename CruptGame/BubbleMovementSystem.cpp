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
	for (Entity entity : m_Entities)
	{
		TransformComponent& transComp = coordinator->GetComponent<TransformComponent>(entity);
		BubbleComponent& bubbleComp = coordinator->GetComponent<BubbleComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
		bubbleComp.totalTime += dt;

		movPhysicsComp.m_Velocity.y = 50.f;
		movPhysicsComp.m_Velocity.x = sin(5.f * bubbleComp.totalTime) * 20.f;

		transComp.position.y -= movPhysicsComp.m_Velocity.y * dt * boxComp.m_EntryTimeY;
		transComp.position.x += movPhysicsComp.m_Velocity.x * dt;

		boxComp.m_CollisionRect.x = int(transComp.position.x);
		boxComp.m_CollisionRect.y = int(transComp.position.y);

	}
}
