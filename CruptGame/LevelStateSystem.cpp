#include "LevelStateSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "SignalHandler.h"
#include "SceneManager.h"

crupt::LevelStateSystem::~LevelStateSystem()
{
}

void crupt::LevelStateSystem::Init()
{
	m_TransitionTime = 1.f;
	m_TotalTime = 0.f;
	SignalHandler<LevelStateComponent>::GetInstance().AddListener(std::bind(&crupt::LevelStateSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::LevelStateSystem::SetMap(Entity mapEntity)
{
	m_MapEntity = mapEntity;
}

void crupt::LevelStateSystem::Update(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

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
			SceneManager::GetInstance().SetActiveScene(L"BBMainMenuScene");
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
