#include "CruptEnginePCH.h"
#include "BBMainMenuScene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameSystems.h"
#include "GameComponents.h"
#include "SceneManager.h"
#include <SDL.h>


#include <iostream>

crupt::BBMainMenuScene::BBMainMenuScene()
	: GameScene(L"BBMainMenuScene")
{
}

void crupt::BBMainMenuScene::Init()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};

	//UI
	Entity menu = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(menu, RenderableComponent{ResourceManager::GetInstance().LoadTexture("MainMenu.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(menu, TransformComponent{glm::vec3(0.f,0.f,0.f)});
}

void crupt::BBMainMenuScene::Update(float dt)
{
	m_TotalTime += dt;
	crupt::SceneManager& sceneManager = SceneManager::GetInstance();
	if(m_TotalTime >= 2.f)
	{
		sceneManager.SetActiveScene(L"BubbleBobbleScene");
		m_TotalTime = 0.f;
	}
}

void crupt::BBMainMenuScene::PostUpdate(float dt)
{
	dt;
}

void crupt::BBMainMenuScene::Render()
{	
	m_pRenderSystem->Render();
}

