#include "LevelStateSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "SignalHandler.h"

crupt::LevelStateSystem::~LevelStateSystem()
{
}

void crupt::LevelStateSystem::Init()
{
	SignalHandler<LevelStateComponent>::GetInstance().AddListener(std::bind(&crupt::LevelStateSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::LevelStateSystem::SetMap(Entity mapEntity)
{
	m_MapEntity = mapEntity;
}

void crupt::LevelStateSystem::Update(float)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	if(m_Entities.size() <= 0)
	{
		TileMapComponent& tileComp = coordinator->GetComponent<TileMapComponent>(m_MapEntity);

		if(tileComp.currentLevel < tileComp.totalLevels)
		{
			tileComp.currentLevel++;
		}
		else
		{
			//END GAME. (Go To End Screen)
			return;
		}

		LevelStateComponent stateComp;
		stateComp.currentLevel = tileComp.currentLevel;
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
