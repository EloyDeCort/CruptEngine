#include "CruptEnginePCH.h"
#include "JumpSystem.h"
#include "ECSCoordinator.h"
#include "SoundManager.h"
#include "SignalHandler.h"

crupt::JumpSystem::JumpSystem()
{
	//INIT Audio
	FMOD_RESULT fmodResult;
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	fmodResult = pFmodSystem->createStream("../Data/Sound/SFX/Jump.wav", FMOD_DEFAULT, 0, &m_pSoundEffect);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
	m_pSoundEffect->setMode(FMOD_LOOP_OFF);
}

crupt::JumpSystem::~JumpSystem()
{
}

void crupt::JumpSystem::Init()
{
	SignalHandler<JumpComponent>::GetInstance().AddListener(std::bind(&crupt::JumpSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::JumpSystem::OnDispatch(const JumpComponent& component)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(component.target);
	BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(component.target);

	if(coordinator->HasComponent<PlayerStateComponent>(component.target))
	{
		HealthComponent& healthComp = coordinator->GetComponent<HealthComponent>(component.target);

		if(healthComp.dead)
			return;

		if(boxComp.colDirY == eDirection::DOWN)
		{
			////Play Sound
			auto pFmodSystem = SoundManager::GetInstance().GetSystem();
			FMOD_RESULT fmodResult;
			fmodResult = pFmodSystem->playSound(m_pSoundEffect, nullptr, false , &m_pChannel);
			SoundManager::GetInstance().ErrorCheck(fmodResult);

			movPhysicsComp.force.y = component.jumpHeight;
		}

		return;
	}
	
	if(boxComp.colDirY == eDirection::DOWN)
	{
		movPhysicsComp.force.y = component.jumpHeight;
	}
	
}
