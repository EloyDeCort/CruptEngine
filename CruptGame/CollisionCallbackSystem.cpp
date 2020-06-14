#include "CruptEnginePCH.h"
#include "CollisionCallbackSystem.h"
#include "PlayerStateSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"
#include "SoundManager.h"

crupt::CollisionCallbackSystem::CollisionCallbackSystem()
{
	//INIT Audio
	FMOD_RESULT fmodResult;
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	fmodResult = pFmodSystem->createStream("../Data/Sound/SFX/Pickup.wav", FMOD_DEFAULT, 0, &m_pPickupEffect);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
	m_pPickupEffect->setMode(FMOD_LOOP_OFF);

	fmodResult = pFmodSystem->createStream("../Data/Sound/SFX/Damage.wav", FMOD_DEFAULT, 0, &m_pDamageEffect);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
	m_pDamageEffect->setMode(FMOD_LOOP_OFF);
}

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
		else if(m_pCoordinator->HasComponent<MaitaComponent>(entity))
		{
			callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnMaitaCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
		}
		else if(m_pCoordinator->HasComponent<DropComponent>(entity))
		{
			callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnDropCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
		}
		else if(m_pCoordinator->HasComponent<BoulderComponent>(entity))
		{
			callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnBoulderCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
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
	else if(m_pCoordinator->HasComponent<MaitaComponent>(entity))
	{
		callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnMaitaCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	}
	else if(m_pCoordinator->HasComponent<DropComponent>(entity))
	{
		callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnDropCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	}
	else if(m_pCoordinator->HasComponent<BoulderComponent>(entity))
	{
		callBackComp.onCollision = std::bind(&crupt::CollisionCallbackSystem::OnBoulderCallback, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	}
}

void crupt::CollisionCallbackSystem::OnPlayerCallback(Entity self, Entity collider, eDirection direction)
{
	HealthComponent& healthComp = m_pCoordinator->GetComponent<HealthComponent>(self);
	if(healthComp.dead)
		return;

	if(m_pCoordinator->HasComponent<BubbleComponent>(collider))
	{
		//std::cout << int(direction) << std::endl;
		if(direction != eDirection::DOWN)
		{
			m_pCoordinator->GetComponent<BubbleComponent>(collider).shouldPop = true;
		}
		else
		{
			//m_pCoordinator->GetComponent<TransformComponent>(self).position = m_pCoordinator->GetComponent<TransformComponent>(collider).position;
		}
	
	}
	else if(m_pCoordinator->HasComponent<EnemyComponent>(collider))
	{
		if(healthComp.gotHit)
			return;

		//Play Sound
		auto pFmodSystem = SoundManager::GetInstance().GetSystem();
		FMOD_RESULT fmodResult;
		fmodResult = pFmodSystem->playSound(m_pDamageEffect, nullptr, false , &m_pChannel);
		SoundManager::GetInstance().ErrorCheck(fmodResult);

		healthComp.currentHealth--;
		healthComp.gotHit = true;
		if(healthComp.currentHealth <= 0)
		{
			healthComp.dead = true;
			return;
		}
	}
	else if(m_pCoordinator->HasComponent<DropComponent>(collider))
	{
		//Play Sound
		auto pFmodSystem = SoundManager::GetInstance().GetSystem();
		FMOD_RESULT fmodResult;
		fmodResult = pFmodSystem->playSound(m_pPickupEffect, nullptr, false , &m_pChannel);
		SoundManager::GetInstance().ErrorCheck(fmodResult);

		DropComponent& dropComp = m_pCoordinator->GetComponent<DropComponent>(collider);
		ScoreComponent& playerScore = m_pCoordinator->GetComponent<ScoreComponent>(self);

		playerScore.score += dropComp.score;
		dropComp.destroy = true;

		SignalHandler<ScoreComponent>::GetInstance().Publish(playerScore);
	}
	else if(m_pCoordinator->HasComponent<BoulderComponent>(collider))
	{
		if(healthComp.gotHit)
			return;

		//Play Sound
		auto pFmodSystem = SoundManager::GetInstance().GetSystem();
		FMOD_RESULT fmodResult;
		fmodResult = pFmodSystem->playSound(m_pDamageEffect, nullptr, false , &m_pChannel);
		SoundManager::GetInstance().ErrorCheck(fmodResult);

		healthComp.currentHealth--;
		healthComp.gotHit = true;
		if(healthComp.currentHealth <= 0)
		{
			healthComp.dead = true;
			return;
		}
	}
}

void crupt::CollisionCallbackSystem::OnBubbleCallback(Entity , Entity , eDirection )
{
	
}

void crupt::CollisionCallbackSystem::OnZenchanCallback(Entity self, Entity collider, eDirection)
{
	if(m_pCoordinator->HasComponent<BubbleComponent>(collider))
	{
		const BubbleStateComponent& bubbleStateComp = m_pCoordinator->GetComponent<BubbleStateComponent>(collider);
		if(bubbleStateComp.animationState != BubbleAnimState::NORMAL)
			return;

		m_pCoordinator->GetComponent<ZenchanComponent>(self).shouldDie = true;
		
		BubbleStateComponent stateComp;
		stateComp.target = collider;
		stateComp.animationState = BubbleAnimState::ZENCHAN;

 		SignalHandler<BubbleStateComponent>::GetInstance().Publish(stateComp);
	}
}

void crupt::CollisionCallbackSystem::OnMaitaCallback(Entity self, Entity collider, eDirection)
{
	if(m_pCoordinator->HasComponent<BubbleComponent>(collider))
	{
		const BubbleStateComponent& bubbleStateComp = m_pCoordinator->GetComponent<BubbleStateComponent>(collider);
		if(bubbleStateComp.animationState != BubbleAnimState::NORMAL)
			return;

		m_pCoordinator->GetComponent<MaitaComponent>(self).shouldDie = true;
		
		BubbleStateComponent stateComp;
		stateComp.target = collider;
		stateComp.animationState = BubbleAnimState::MAITA;

		SignalHandler<BubbleStateComponent>::GetInstance().Publish(stateComp);
	}
}

void crupt::CollisionCallbackSystem::OnBoulderCallback(Entity self, Entity, eDirection direction)
{
	if(direction == eDirection::LEFT || direction == eDirection::RIGHT)
	{
		m_pCoordinator->GetComponent<BoulderComponent>(self).shouldDestroy = true;
	}
}

void crupt::CollisionCallbackSystem::OnDropCallback(Entity, Entity , eDirection )
{
	
}
