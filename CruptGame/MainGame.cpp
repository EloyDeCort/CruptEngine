#include "MainGame.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "BubbleBobbleScene.h"

void crupt::MainGame::InitGame()
{
	RegisterAdditionalComponents();
	RegisterAdditionalSystems();

	crupt::SceneManager& sceneManager = SceneManager::GetInstance();

	//Adding of scenes
	sceneManager.AddScene(new DemoScene());
	sceneManager.AddScene(new BubbleBobbleScene());
	sceneManager.SetActiveScene(L"BubbleBobbleScene");

	sceneManager.Init();
}

void crupt::MainGame::RegisterAdditionalComponents()
{
	//For Components that do not belong in the main engine.

}

void crupt::MainGame::RegisterAdditionalSystems()
{
	//For handling components that do not belong in the main engine.

}
