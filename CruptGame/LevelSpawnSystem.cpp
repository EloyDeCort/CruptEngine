#include "CruptEnginePCH.h"
#include "LevelSpawnSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"

crupt::LevelSpawnSystem::~LevelSpawnSystem()
{
}

void crupt::LevelSpawnSystem::Init()
{
	SignalHandler<LevelStateComponent>::GetInstance().AddListener(std::bind(&crupt::LevelSpawnSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::LevelSpawnSystem::OnDispatch(const LevelStateComponent& component )
{
	SpawnEnemyComponent enemyComp;

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
