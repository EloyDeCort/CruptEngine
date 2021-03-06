#include "CruptEnginePCH.h"
#include "BubbleMovementSystem.h"
#include "ResourceManager.h"
#include "SignalHandler.h"

using namespace crupt;

BubbleMovementSystem::~BubbleMovementSystem()
{
	
}

void BubbleMovementSystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
}

void BubbleMovementSystem::Update(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;

	for (Entity entity : m_Entities)
	{
		//Handle Bubbles based on their current state.
		TransformComponent& transComp = coordinator->GetComponent<TransformComponent>(entity);
		BubbleStateComponent& bubbleStateComp = coordinator->GetComponent<BubbleStateComponent>(entity);
		BubbleComponent& bubbleComp = coordinator->GetComponent<BubbleComponent>(entity);

		if(bubbleComp.shouldPop)
		{
			SpawnDrop(bubbleStateComp.animationState, transComp.position);
			toDelete.push_back(entity);
			continue;
		}

		if(bubbleStateComp.animationState == BubbleAnimState::ZENCHAN)
		{
			if(bubbleComp.totalTime > bubbleComp.maxLifeTime)
			{
				bubbleComp.totalTime = 0.f;
				SpawnEnemy(bubbleStateComp.animationState, transComp.position);
				toDelete.push_back(entity);
				continue;
			}
		}
		else if(bubbleStateComp.animationState == BubbleAnimState::MAITA)
		{
			if(bubbleComp.totalTime > bubbleComp.maxLifeTime)
			{
				bubbleComp.totalTime = 0.f;
				SpawnEnemy(bubbleStateComp.animationState, transComp.position);
				toDelete.push_back(entity);
				continue;
			}
		}

		

		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);

		transComp.position.x += movPhysicsComp.velocity.x * dt * boxComp.entryTimeX;
		transComp.position.y += movPhysicsComp.velocity.y * dt * boxComp.entryTimeY;

		boxComp.collisionRect.x = int(transComp.position.x);
		boxComp.collisionRect.y = int(transComp.position.y);

		if(boxComp.entryTimeY < 0.001f)
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
	//Updating the bubbles their movement
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	for (Entity entity : m_Entities)
	{
		BubbleComponent& bubbleComp = coordinator->GetComponent<BubbleComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);

		bubbleComp.totalTime += dt;


		if(bubbleComp.totalTime < bubbleComp.xSpeedTime)
		{
			movPhysicsComp.velocity.x = sin(5.f * bubbleComp.totalTime) * 20.f + bubbleComp.xSpeed;

			if(bubbleComp.flipped)
			{
				movPhysicsComp.velocity.x = -movPhysicsComp.velocity.x;
			}
		}
		else
		{
			movPhysicsComp.velocity.x = sin(5.f * bubbleComp.totalTime) * 20.f ;
			movPhysicsComp.velocity.y = -50.f;
		}
	}
}


void crupt::BubbleMovementSystem::SpawnEnemy(BubbleAnimState state, const glm::vec2& pos)
{
	//Spawning an enemy
	SpawnEnemyComponent enemyComp;
	switch(state)
	{
	case BubbleAnimState::ZENCHAN:
		enemyComp.type = EnemyType::ZENCHAN;
		break;
	case BubbleAnimState::MAITA:
		enemyComp.type = EnemyType::MAITA;
		break;
	}
	enemyComp.spawnPos = pos;
	SignalHandler<SpawnEnemyComponent>::GetInstance().Publish(enemyComp);
}

void crupt::BubbleMovementSystem::SpawnDrop(BubbleAnimState state, const glm::vec2& pos)
{
	//Spawning a drop
	DropComponent dropComp;
	switch(state)
	{
	case BubbleAnimState::ZENCHAN:
		dropComp.type = DropType::MELON;
		break;
	case BubbleAnimState::MAITA:
		dropComp.type = DropType::FRIES;
		break;
	}
	dropComp.pos = pos;
	SignalHandler<DropComponent>::GetInstance().Publish(dropComp);
}
