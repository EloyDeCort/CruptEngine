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
	sceneManager.SetActiveScene(L"BubbleBobbleScene");
}

void crupt::MainGame::RegisterAdditionalComponents()
{
	//For Components that do not belong in the main engine.
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.RegisterComponent<PlayerStateComponent>();
	pCoordinator.RegisterComponent<JumpComponent>();
	pCoordinator.RegisterComponent<MoveComponent>();
	pCoordinator.RegisterComponent<BubbleComponent>();
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

	pBubbleMovementSystem->Init();
}
