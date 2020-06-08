#include "CruptEnginePCH.h"
#include "BubbleBobbleScene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameSystems.h"
#include "GameComponents.h"
#include <SDL.h>


#include <iostream>

crupt::BubbleBobbleScene::BubbleBobbleScene()
	: GameScene(L"BubbleBobbleScene")
	, m_FpsCounter{}
{

}

void crupt::BubbleBobbleScene::Init()
{
	InitSystems();
	InitEntities();

}

void crupt::BubbleBobbleScene::InitSystems()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	//INITIALIZE THE SYSTEMS
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	m_pTextSystem = pCoordinator.GetSystem<TextSystem>();
	m_pFPSSystem = pCoordinator.GetSystem<FPSSystem>();
	m_pTileMapSystem = pCoordinator.GetSystem<TileMapSystem>();
	m_pSpriteSystem = pCoordinator.GetSystem<SpriteSystem>();
	//m_pJumpSystem = pCoordinator.GetSystem<JumpSystem>();	
}

void crupt::BubbleBobbleScene::InitEntities()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};
	//Simple FPS component
	crupt::Font* fontFps = ResourceManager::GetInstance().LoadFont("Bobble.ttf", 16);
	m_FpsCounter = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(m_FpsCounter, RenderableComponent{});
	pCoordinator.AddComponent<TransformComponent>(m_FpsCounter, TransformComponent{glm::vec3(90.f,25.f,0.f)});
	pCoordinator.AddComponent<TextComponent>(m_FpsCounter, TextComponent{bool{true}, std::string("FPS:"), fontFps, glm::vec3(255.f, 255.f, 255.f)});
	pCoordinator.AddComponent<FPSComponent>(m_FpsCounter, FPSComponent{});

	//Init the map entity
	Entity map = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TileMapComponent>(map, TileMapComponent{});

	//UI
	Entity ui = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(ui, RenderableComponent{ResourceManager::GetInstance().LoadTexture("UI.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(ui, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	//Player
	SpriteComponent spriteComp{};
	spriteComp.m_FrameCount = 7;
	spriteComp.m_AnimationRate = 12; 
	Entity player1 = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<SpriteComponent>(player1, spriteComp);
	pCoordinator.AddComponent<RenderableComponent>(player1, RenderableComponent{ResourceManager::GetInstance().LoadTexture("Bubblun.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(player1, TransformComponent{glm::vec3(0.f,0.f,0.f)});
}

void crupt::BubbleBobbleScene::Update(float dt)
{	
	m_pFPSSystem->SetText(m_FpsCounter, "FPS:" + std::to_string(m_pFPSSystem->GetFPS(m_FpsCounter)));
	m_pTextSystem->Update(dt);
	m_pFPSSystem->Update(m_FpsCounter, dt);
	m_pSpriteSystem->Update(dt);
}

void crupt::BubbleBobbleScene::Render()
{	
	m_pTileMapSystem->Render();
	m_pRenderSystem->Render();
}

