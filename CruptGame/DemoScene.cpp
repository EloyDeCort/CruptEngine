#include "CruptEnginePCH.h"
#include "DemoScene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/TileLayer.hpp>


#include <iostream>

crupt::DemoScene::DemoScene()
	: GameScene(L"DemoScene")
	, m_FpsCounter{}
{

}

void crupt::DemoScene::Init()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	Entity background = pCoordinator.CreateEntity();

	//INITIALIZE THE SYSTEMS
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	m_pTextSystem = pCoordinator.GetSystem<TextSystem>();
	m_pFPSSystem = pCoordinator.GetSystem<FPSSystem>();
	m_pJumpSystem = pCoordinator.GetSystem<JumpSystem>();


	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};
	pCoordinator.AddComponent<RenderableComponent>(background, RenderableComponent{ResourceManager::GetInstance().LoadTexture("background.jpg",renderer)});
	pCoordinator.AddComponent<TransformComponent>(background, TransformComponent{glm::vec3(0.f,0.f,0.f)});


	crupt::Font* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	Entity text = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(text, RenderableComponent{});
	pCoordinator.AddComponent<TransformComponent>(text, TransformComponent{glm::vec3(80.f,20.f,0.f)});
	pCoordinator.AddComponent<TextComponent>(text, TextComponent{bool{true}, std::string("Programming 4 Assignment"), font, glm::vec3(255.f, 0.f, 0.f)});

	crupt::Font* fontFps = ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	m_FpsCounter = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(m_FpsCounter, RenderableComponent{});
	pCoordinator.AddComponent<TransformComponent>(m_FpsCounter, TransformComponent{glm::vec3(0.f,0.f,0.f)});
	pCoordinator.AddComponent<TextComponent>(m_FpsCounter, TextComponent{bool{true}, std::string("FPS: "), fontFps, glm::vec3(255.f, 255.f, 0.f)});
	pCoordinator.AddComponent<FPSComponent>(m_FpsCounter, FPSComponent{});

	Entity player = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TransformComponent>(player, TransformComponent{glm::vec3(216.f,180.f,0.f)});

    Entity logo = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(logo, RenderableComponent{ResourceManager::GetInstance().LoadTexture("logo.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(logo, TransformComponent{glm::vec3(216.f,180.f,0.f)});
    InputManager::GetInstance().SetPlayer(logo);

}

void crupt::DemoScene::Update(float dt)
{	
	m_pFPSSystem->SetText(m_FpsCounter, "FPS: " + std::to_string(m_pFPSSystem->GetFPS(m_FpsCounter)));
	m_pTextSystem->Update(dt);
	m_pFPSSystem->Update(m_FpsCounter, dt);
	m_pJumpSystem->Update(dt);

}

void crupt::DemoScene::Render()
{
	m_pRenderSystem->Render();
}
