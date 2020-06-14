#include "CruptEnginePCH.h"
#include "LevelSpawnSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"

crupt::LevelSpawnSystem::~LevelSpawnSystem()
{
}

void crupt::LevelSpawnSystem::Init()
{
	m_NrOfPlayers = 0;
	m_Player1SpawnLoc = {50.f,Settings::windowHeight - 70.f};
	m_Player2SpawnLoc = {560.f,Settings::windowHeight - 70.f};
	SignalHandler<LevelStateComponent>::GetInstance().AddListener(std::bind(&crupt::LevelSpawnSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::LevelSpawnSystem::Reset()
{
	m_NrOfPlayers = 0;
}

void crupt::LevelSpawnSystem::SetPlayer1(Entity player)
{
	m_Player1 = player;
	m_NrOfPlayers++;
}

void crupt::LevelSpawnSystem::SetPlayer2(Entity player)
{
	m_Player2 = player;
	m_NrOfPlayers++;
}

void crupt::LevelSpawnSystem::OnDispatch(const LevelStateComponent& component )
{
	SpawnEnemyComponent enemyComp;
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();

	if(m_NrOfPlayers >= 1)
	{
		TransformComponent& transComp = pCoordinator.GetComponent<TransformComponent>(m_Player1);
		transComp.position = m_Player1SpawnLoc;
	}

	if(m_NrOfPlayers >= 2)
	{
		TransformComponent& transComp = pCoordinator.GetComponent<TransformComponent>(m_Player2);
		transComp.position = m_Player2SpawnLoc;
	}


	switch(component.currentLevel)
	{
	case 1:
		enemyComp.type = EnemyType::ZENCHAN;
		enemyComp.spawnPos = glm::vec2{Settings::windowWidth/2.f,100.f};
		SignalHandler<SpawnEnemyComponent>::GetInstance().Publish(enemyComp);
		break;
	case 2:
		enemyComp.type = EnemyType::ZENCHAN;
		enemyComp.spawnPos = glm::vec2{Settings::windowWidth/2.f,100.f};
		SignalHandler<SpawnEnemyComponent>::GetInstance().Publish(enemyComp);
		break;
	case 3:
		enemyComp.type = EnemyType::ZENCHAN;
		enemyComp.spawnPos = glm::vec2{Settings::windowWidth/2.f,100.f};
		SignalHandler<SpawnEnemyComponent>::GetInstance().Publish(enemyComp);
		break;
	}
}
