#include "CruptEnginePCH.h"
#include "PlayerStateSystem.h"
#include "GameComponents.h"
#include "Components.h"
#include "ECSCoordinator.h"
#include "BBLoseScene.h"
#include "SceneManager.h"
using namespace crupt;

PlayerStateSystem::~PlayerStateSystem()
{
	
}

void PlayerStateSystem::Init()
{
	m_NrOfPlayers = 0;
	m_SpitTime = 0.f;
	m_MaxSpitTime = 0.2f;
}

void PlayerStateSystem::Update(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;
	for (Entity entity : m_Entities)
	{
		
		PlayerStateComponent& stateComp = coordinator->GetComponent<PlayerStateComponent>(entity);
		SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		RenderableComponent& renderable = coordinator->GetComponent<RenderableComponent>(entity);
		HealthComponent& healthComp = coordinator->GetComponent<HealthComponent>(entity);

		if(m_NrOfPlayers == 1)
		{
			HealthComponent& healthCompP1 = coordinator->GetComponent<HealthComponent>(m_Player1);
			if(healthCompP1.dead)
			{
				//END GAME. (Go To End Screen)
				BBLoseScene* loseScene = reinterpret_cast<BBLoseScene*>(SceneManager::GetInstance().GetScene(L"BBLoseScene"));
				ScoreComponent& scoreComp1 = coordinator->GetComponent<ScoreComponent>(m_Player1);
				loseScene->SetScoreP1(scoreComp1.score);

				SceneManager::GetInstance().SetActiveScene(L"BBLoseScene");
				return;
			}
		}
		else if(m_NrOfPlayers == 2)
		{
			HealthComponent& healthCompP1 = coordinator->GetComponent<HealthComponent>(m_Player1);
			HealthComponent& healthCompP2 = coordinator->GetComponent<HealthComponent>(m_Player2);

			if(healthCompP1.dead && healthCompP2.dead)
			{
				//END GAME. (Go To End Screen)
				BBLoseScene* loseScene = reinterpret_cast<BBLoseScene*>(SceneManager::GetInstance().GetScene(L"BBLoseScene"));
				
				ScoreComponent& scoreComp1 = coordinator->GetComponent<ScoreComponent>(m_Player1);
				loseScene->SetScoreP1(scoreComp1.score);

				ScoreComponent& scoreComp2 = coordinator->GetComponent<ScoreComponent>(m_Player2);
				loseScene->SetScoreP2(scoreComp2.score);

				SceneManager::GetInstance().SetActiveScene(L"BBLoseScene");
				return;
			}
		}

		/*if(healthComp.dead)
		{
			toDelete.push_back(entity);
			continue;
		}*/

		if(healthComp.gotHit)
		{
			healthComp.totalTime += dt;
			if(healthComp.totalTime >= healthComp.hitDelay)
			{
				healthComp.gotHit = false;
				healthComp.totalTime = 0.f;
			}
		}


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

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}

void crupt::PlayerStateSystem::SetPlayer1(Entity player)
{
	m_Player1 = player;
	m_NrOfPlayers++;
}

void crupt::PlayerStateSystem::SetPlayer2(Entity player)
{
	m_Player2 = player;
	m_NrOfPlayers++;
}

void crupt::PlayerStateSystem::Reset()
{
	m_NrOfPlayers = 0;
}
