#include "CollisionCallbackSystem.h"
#include "CruptEnginePCH.h"
#include "PlayerStateSystem.h"
#include "GameComponents.h"
#include "Components.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"


crupt::CollisionCallbackSystem::~CollisionCallbackSystem()
{
}

void crupt::CollisionCallbackSystem::Init()
{
	m_pCoordinator = &ECSCoordinator::GetInstance();

	for (const Entity& entity : m_Entities)
	{	
		CollisionCallbackComponent& callBackComp = m_pCoordinator->GetComponent<CollisionCallbackComponent>(entity);
		//checking if the entity is a player
		if(m_pCoordinator->HasComponent<PlayerStateComponent>(entity))
		{
			callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnPlayerCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
		}
		//Check if its a bubble
		else if(m_pCoordinator->HasComponent<BubbleComponent>(entity))
		{
			callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnBubbleCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
		}
		else if(m_pCoordinator->HasComponent<ZenchanComponent>(entity))
		{
			callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnZenchanCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
		}
		else if(m_pCoordinator->HasComponent<DropComponent>(entity))
		{
			callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnDropCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
		}
	}
}

void crupt::CollisionCallbackSystem::AddEntityCallback(Entity entity)
{
	CollisionCallbackComponent& callBackComp = m_pCoordinator->GetComponent<CollisionCallbackComponent>(entity);
	//checking if the entity is a player
	if(m_pCoordinator->HasComponent<PlayerStateComponent>(entity))
	{
		callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnPlayerCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	}
	//Check if its a bubble
	else if(m_pCoordinator->HasComponent<BubbleComponent>(entity))
	{
		callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnBubbleCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	}
	else if(m_pCoordinator->HasComponent<ZenchanComponent>(entity))
	{
		callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnZenchanCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	}
	else if(m_pCoordinator->HasComponent<DropComponent>(entity))
	{
		callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnDropCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	}
}

void crupt::CollisionCallbackSystem::OnPlayerCallback(Entity self, Entity collider, eDirection direction)
{
	if(m_pCoordinator->HasComponent<BubbleComponent>(collider))
	{
		//std::cout << int(direction) << std::endl;
		if(direction != eDirection::DOWN)
		{
			m_pCoordinator->GetComponent<BubbleComponent>(collider).shouldPop = true;
		}
		else
		{
			self;
			//m_pCoordinator->GetComponent<TransformComponent>(self).position = m_pCoordinator->GetComponent<TransformComponent>(collider).position;
		}
	
	}
	else if(m_pCoordinator->HasComponent<EnemyComponent>(collider))
	{
		HealthComponent& healthComp = m_pCoordinator->GetComponent<HealthComponent>(self);
		if(healthComp.gotHit)
			return;

		healthComp.currentHealth--;
		healthComp.gotHit = true;
		if(healthComp.currentHealth <= 0)
		{
			healthComp.dead = true;
		}
	}
	else if(m_pCoordinator->HasComponent<DropComponent>(collider))
	{
		DropComponent& dropComp = m_pCoordinator->GetComponent<DropComponent>(collider);
		ScoreComponent& playerScore = m_pCoordinator->GetComponent<ScoreComponent>(self);

		playerScore.score += dropComp.score;

		SignalHandler<ScoreComponent>::GetInstance().Publish(playerScore);
	}
}

void crupt::CollisionCallbackSystem::OnBubbleCallback(Entity , Entity , eDirection )
{
	/*std::cout << int(direction) << std::endl;
	if(direction == eDirection::UP || direction == eDirection::DOWN)
	{

	}*/
}

void crupt::CollisionCallbackSystem::OnZenchanCallback(Entity self, Entity collider, eDirection)
{
	if(m_pCoordinator->HasComponent<BubbleComponent>(collider))
	{
		m_pCoordinator->GetComponent<ZenchanComponent>(self).shouldDie = true;
		
		BubbleStateComponent stateComp;
		stateComp.target = collider;
		stateComp.animationState = BubbleAnimState::ZENCHAN;

		SignalHandler<BubbleStateComponent>::GetInstance().Publish(stateComp);
	}
}

void crupt::CollisionCallbackSystem::OnDropCallback(Entity, Entity , eDirection )
{
	
}
