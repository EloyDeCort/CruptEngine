#include "CruptEnginePCH.h"
#include "MainGame.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "BubbleBobbleScene.h"
#include "BBMainMenuScene.h"
#include "GameComponents.h"
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
	pCoordinator.RegisterComponent<TileMapComponent>();
}

void crupt::MainGame::RegisterAdditionalSystems()
{
	//For handling components that do not belong in the main engine.
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();

	//Register the systems
	TileMapSystem* tileMapSystem = pCoordinator.RegisterSystem<TileMapSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TileMapComponent>());
		pCoordinator.SetSystemSignature<TileMapSystem>(signature);
	}
	tileMapSystem->Init(m_pRenderSystem->GetSDLRenderer());
}
