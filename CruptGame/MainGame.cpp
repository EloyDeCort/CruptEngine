#include "CruptEnginePCH.h"
#include "MainGame.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "BubbleBobbleScene.h"
#include "BBMainMenuScene.h"
#include "GameComponents.h"
#include "Components.h"
#include "GameSystems.h"

void crupt::MainGame::InitGame()
{
	RegisterAdditionalComponents();
	RegisterAdditionalSystems();

	crupt::SceneManager& sceneManager = SceneManager::GetInstance();

	//Adding of scenes
	sceneManager.AddScene(new BBMainMenuScene());
	sceneManager.AddScene(new BubbleBobbleScene());
	sceneManager.SetActiveScene(L"BBMainMenuScene");
}

void crupt::MainGame::RegisterAdditionalComponents()
{
	//For Components that do not belong in the main engine.
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.RegisterComponent<PlayerStateComponent>();
	pCoordinator.RegisterComponent<JumpComponent>();
	pCoordinator.RegisterComponent<MoveComponent>();
	pCoordinator.RegisterComponent<BubbleComponent>();
	pCoordinator.RegisterComponent<ZenchanComponent>();
	pCoordinator.RegisterComponent<BubbleStateComponent>();
	pCoordinator.RegisterComponent<SpawnEnemyComponent>();
	pCoordinator.RegisterComponent<HealthComponent>();
	pCoordinator.RegisterComponent<EnemyComponent>();
	pCoordinator.RegisterComponent<DropComponent>();
	pCoordinator.RegisterComponent<ScoreComponent>();
	pCoordinator.RegisterComponent<LevelStateComponent>();
	pCoordinator.RegisterComponent<EntityComponent>();
}

void crupt::MainGame::RegisterAdditionalSystems()
{
	//For handling components that do not belong in the main engine.
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();

	//Register the systems

	JumpSystem* pJumpSystem = pCoordinator.RegisterSystem<JumpSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<JumpComponent>());
		pCoordinator.SetSystemSignature<JumpSystem>(signature);
	}
	pJumpSystem->Init();

	MoveSystem* pMoveSystem = pCoordinator.RegisterSystem<MoveSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<MoveComponent>());
		pCoordinator.SetSystemSignature<MoveSystem>(signature);
	}
	pMoveSystem->Init();

	PlayerStateSystem* pPlayerStateSystem = pCoordinator.RegisterSystem<PlayerStateSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<PlayerStateComponent>());
		pCoordinator.SetSystemSignature<PlayerStateSystem>(signature);
	}
	pPlayerStateSystem->Init();

	BubbleSpawnSystem* pBubbleSpawnSystem = pCoordinator.RegisterSystem<BubbleSpawnSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<BubbleComponent>());
		pCoordinator.SetSystemSignature<BubbleSpawnSystem>(signature);
	}

	pBubbleSpawnSystem->Init(m_pRenderSystem->GetSDLRenderer());

	BubbleMovementSystem* pBubbleMovementSystem = pCoordinator.RegisterSystem<BubbleMovementSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<BubbleComponent>());
		pCoordinator.SetSystemSignature<BubbleMovementSystem>(signature);
	}

	pBubbleMovementSystem->Init(m_pRenderSystem->GetSDLRenderer());

	CollisionCallbackSystem* colCallbackSystem = pCoordinator.RegisterSystem<CollisionCallbackSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<CollisionCallbackComponent>());
		pCoordinator.SetSystemSignature<CollisionCallbackSystem>(signature);
	}

	ZenchanMovementSystem* pMaitaMovementSystem = pCoordinator.RegisterSystem<ZenchanMovementSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<ZenchanComponent>());
		pCoordinator.SetSystemSignature<ZenchanMovementSystem>(signature);
	}
	pMaitaMovementSystem->Init();

	BubbleStateSystem* pBubbleStateSystem = pCoordinator.RegisterSystem<BubbleStateSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<BubbleStateComponent>());
		pCoordinator.SetSystemSignature<BubbleStateSystem>(signature);
	}
	pBubbleStateSystem->Init();

	SpawnEnemySystem* pSpawnEnemySystem = pCoordinator.RegisterSystem<SpawnEnemySystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<SpawnEnemyComponent>());
		pCoordinator.SetSystemSignature<SpawnEnemySystem>(signature);
	}
	pSpawnEnemySystem->Init(m_pRenderSystem->GetSDLRenderer(), colCallbackSystem);

	HealthDisplaySystem* pHealthDisplaySystem = pCoordinator.RegisterSystem<HealthDisplaySystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<HealthComponent>());
		pCoordinator.SetSystemSignature<HealthDisplaySystem>(signature);
	}
	pHealthDisplaySystem->Init(m_pRenderSystem->GetSDLRenderer());

	SpawnDropSystem* pSpawnDropSystem = pCoordinator.RegisterSystem<SpawnDropSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<DropComponent>());
		pCoordinator.SetSystemSignature<SpawnDropSystem>(signature);
	}

	pSpawnDropSystem->Init(m_pRenderSystem->GetSDLRenderer(), colCallbackSystem);


	ScoreDisplaySystem* pScoreDisplaySystem = pCoordinator.RegisterSystem<ScoreDisplaySystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<ScoreComponent>());
		pCoordinator.SetSystemSignature<ScoreDisplaySystem>(signature);
	}
	pScoreDisplaySystem->Init(m_pRenderSystem->GetSDLRenderer());

	
	DropMovementSystem* pDropMovementSystem = pCoordinator.RegisterSystem<DropMovementSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<DropComponent>());
		pCoordinator.SetSystemSignature<DropMovementSystem>(signature);
	}

	pDropMovementSystem->Init();


	LevelStateSystem* pLevelStateSystem = pCoordinator.RegisterSystem<LevelStateSystem>();
	{
		Signature signature;
		//Using enemies to check how many are left.
		signature.set(pCoordinator.GetComponentType<EntityComponent>());
		pCoordinator.SetSystemSignature<LevelStateSystem>(signature);
	}

	pLevelStateSystem->Init();

	LevelSpawnSystem* pLevelSpawnSystem = pCoordinator.RegisterSystem<LevelSpawnSystem>();
	{
		Signature signature;
		//Using enemies to check how many are left.
		signature.set(pCoordinator.GetComponentType<LevelStateComponent>());
		pCoordinator.SetSystemSignature<LevelSpawnSystem>(signature);
	}

	pLevelSpawnSystem->Init();

	WorldBorderSystem* pWorldBorderSystem = pCoordinator.RegisterSystem<WorldBorderSystem>();
	{
		Signature signature;
		//Using enemies to check how many are left.
		signature.set(pCoordinator.GetComponentType<GravityComponent>());
		pCoordinator.SetSystemSignature<WorldBorderSystem>(signature);
	}

	pWorldBorderSystem->Init();
}
