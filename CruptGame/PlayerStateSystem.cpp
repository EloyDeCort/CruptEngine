#include "CruptEnginePCH.h"
#include "PlayerStateSystem.h"
#include "GameComponents.h"
#include "Components.h"
#include "ECSCoordinator.h"
using namespace crupt;

PlayerStateSystem::~PlayerStateSystem()
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	for (Entity entity : m_Entities)
	{
		PlayerStateComponent& stateComp = coordinator->GetComponent<PlayerStateComponent>(entity);

		for(size_t i{}; i < stateComp.m_pStateSprites.size(); ++i)
		{
			if(i != size_t(stateComp.m_AnimationState))
			{
				delete stateComp.m_pStateSprites[i].m_Texture;
				stateComp.m_pStateSprites[i].m_Texture = nullptr;
			}
		}

		stateComp.m_pStateSprites.clear();
	}
}

void PlayerStateSystem::Init()
{
}

void PlayerStateSystem::Update(float dt)
{
	dt;
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	for (Entity entity : m_Entities)
	{
		
		PlayerStateComponent& stateComp = coordinator->GetComponent<PlayerStateComponent>(entity);
		SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		RenderableComponent& renderable = coordinator->GetComponent<RenderableComponent>(entity);
		
		if(movPhysicsComp.m_Velocity.x < FLT_EPSILON && movPhysicsComp.m_Velocity.x > -FLT_EPSILON)
		{
			if(boxComp.m_ColDirY == eDirection::DOWN)
			{
				stateComp.m_AnimationState = PlayerAnimState::IDLE;
			
			}
			else
			{
				stateComp.m_AnimationState = PlayerAnimState::IDLE;
				
			}
		}
		else
		{
			if(boxComp.m_ColDirY == eDirection::DOWN)
			{
				stateComp.m_AnimationState = PlayerAnimState::RUNNING;
			
			}
			else
			{
				stateComp.m_AnimationState = PlayerAnimState::IDLE;
			}
		}

		StateSprite& newSprite = stateComp.m_pStateSprites[int(stateComp.m_AnimationState)];
		spriteComp.m_AnimationRate = newSprite.m_SpriteData.m_AnimationRate;
		spriteComp.m_FrameCount = newSprite.m_SpriteData.m_FrameCount;
		renderable.m_Texture = stateComp.m_pStateSprites[int(stateComp.m_AnimationState)].m_Texture;
	
	}
}
