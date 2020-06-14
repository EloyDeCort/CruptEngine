#include "LevelStateSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "SignalHandler.h"
#include "SceneManager.h"
#include "BBWinScene.h"

crupt::LevelStateSystem::~LevelStateSystem()
{
}

void crupt::LevelStateSystem::Init()
{
	m_Versus = false;
	m_TransitionTime = 1.f;
	m_TotalTime = 0.f;
	SignalHandler<LevelStateComponent>::GetInstance().AddListener(std::bind(&crupt::LevelStateSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::LevelStateSystem::SetMap(Entity mapEntity)
{
	m_MapEntity = mapEntity;
}

void crupt::LevelStateSystem::Reset()
{
	m_NrOfPlayers = 0;
}

void crupt::LevelStateSystem::Update(float dt)
{
	//We check per frame the state of the game, ensuring that the game progress flawlessly
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	if(m_Versus)
		return;

	if(m_Entities.size() <= 0)
	{
		m_TotalTime += dt;

		if(m_TotalTime < m_TransitionTime)
			return;

		m_TotalTime = 0.f;

		TileMapComponent& tileComp = coordinator->GetComponent<TileMapComponent>(m_MapEntity);

		if(tileComp.currentLevel < tileComp.totalLevels - 1)
		{
 			tileComp.currentLevel++;
		}
		else
		{
			//END GAME. (Go To End Screen)
			BBWinScene* winScene = reinterpret_cast<BBWinScene*>(SceneManager::GetInstance().GetScene(L"BBWinScene"));

			if(m_NrOfPlayers >= 1)
			{
				ScoreComponent& scoreComp1 = coordinator->GetComponent<ScoreComponent>(m_Player1);
				winScene->SetScoreP1(scoreComp1.score);
			}

			if(m_NrOfPlayers >= 2)
			{
				ScoreComponent& scoreComp2 = coordinator->GetComponent<ScoreComponent>(m_Player2);
				winScene->SetScoreP2(scoreComp2.score);
			}

			SceneManager::GetInstance().SetActiveScene(L"BBWinScene");
			return;
		}

		LevelStateComponent stateComp;
		stateComp.currentLevel = tileComp.currentLevel + 1;
		SignalHandler<LevelStateComponent>::GetInstance().Publish(stateComp);
	}
}

void crupt::LevelStateSystem::OnDispatch(const LevelStateComponent& component)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	coordinator;
	bool shouldEnd = false;
	if(component.nextLevel)
	{
		TileMapComponent& tileComp = coordinator->GetComponent<TileMapComponent>(m_MapEntity);
		tileComp.currentLevel++;

		if(tileComp.currentLevel > tileComp.totalLevels)
		{
			shouldEnd = true;
		}
	}

	if(component.endGame || shouldEnd)
	{
		return;
	}
}

void crupt::LevelStateSystem::SetPlayer1(Entity player)
{
	m_Player1 = player;
	m_NrOfPlayers++;
}

void crupt::LevelStateSystem::SetPlayer2(Entity player)
{
	m_Player2 = player;
	m_NrOfPlayers++;
}

void crupt::LevelStateSystem::SetVersus(bool versus)
{
	m_Versus = versus;
}
