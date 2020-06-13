#include "CruptEnginePCH.h"
#include "PlayerStateSystem.h"
#include "GameComponents.h"
#include "Components.h"
#include "ECSCoordinator.h"
using namespace crupt;

PlayerStateSystem::~PlayerStateSystem()
{
	
}

void PlayerStateSystem::Init()
{
	m_SpitTime = 0.f;
	m_MaxSpitTime = 0.2f;
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
		
		if(stateComp.animationState == PlayerAnimState::SPITBUBBLE)
		{
			m_SpitTime += dt;
			if(m_SpitTime >= m_MaxSpitTime)
			{
				stateComp.animationState = PlayerAnimState::IDLE;
				m_SpitTime = 0.f;
			}
		}
		else
		{
			if(movPhysicsComp.velocity.x < FLT_EPSILON && movPhysicsComp.velocity.x > -FLT_EPSILON)
			{
				if(boxComp.colDirY == eDirection::DOWN)
				{
					stateComp.animationState = PlayerAnimState::IDLE;
			
				}
				else
				{
					stateComp.animationState = PlayerAnimState::IDLE;
				
				}
			}
			else
			{
				if(boxComp.colDirY == eDirection::DOWN)
				{
					stateComp.animationState = PlayerAnimState::RUNNING;
			
				}
				else
				{
					stateComp.animationState = PlayerAnimState::IDLE;
				}
			}
		}
	

		StateSprite& newSprite = stateComp.pStateSprites[int(stateComp.animationState)];
		spriteComp.animationRate = newSprite.spriteData.animationRate;
		spriteComp.frameCount = newSprite.spriteData.frameCount;
		renderable.pTexture = stateComp.pStateSprites[int(stateComp.animationState)].pTexture;
	
	}
}
