#include "CruptEnginePCH.h"
#include "BubbleBobbleScene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameSystems.h"
#include "GameComponents.h"
#include "GameCommands.h"
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
	m_pPhysicsSystem = pCoordinator.GetSystem<PhysicsSystem>();
	m_pCollisionSystem = pCoordinator.GetSystem<CollisionSystem>();
	m_pPlayerStateSystem = pCoordinator.GetSystem<PlayerStateSystem>();
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
	//We pass the map to both systems to optimize the find time. This entity won't change.
	m_pTileMapSystem->InitMap(map);
	m_pCollisionSystem->InitMap(map);

	//UI
	Entity ui = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(ui, RenderableComponent{ResourceManager::GetInstance().LoadTexture("UI.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(ui, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	//Player 1 
	SpriteComponent spriteComp{};
	spriteComp.m_AnimationRate = 12; 
	spriteComp.m_ScaleFactor = 2; 
	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Player/Bob_Idle.png",renderer);

	Entity player1 = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<SpriteComponent>(player1, spriteComp);
	pCoordinator.AddComponent<RenderableComponent>(player1, RenderableComponent{defaultAnim});
	pCoordinator.AddComponent<TransformComponent>(player1, TransformComponent{glm::vec3(100.f,100.f,0.f)});
	pCoordinator.AddComponent<MovePhysicsComponent>(player1, MovePhysicsComponent{});
	pCoordinator.AddComponent<GravityComponent>(player1, GravityComponent{});
	pCoordinator.AddComponent<BoxCollisionComponent>(player1, BoxCollisionComponent{0,0,32,32});
	PlayerStateComponent playerStateComp{};
	playerStateComp.m_AnimationState = PlayerAnimState::IDLE;

	spriteComp.m_FrameCount = 1;
	playerStateComp.m_pStateSprites.push_back(StateSprite{spriteComp,defaultAnim});
	
	spriteComp.m_FrameCount = 8;
	playerStateComp.m_pStateSprites.push_back(StateSprite{spriteComp,ResourceManager::GetInstance().LoadTexture("Player/Bob_Walking.png",renderer)});

	pCoordinator.AddComponent<PlayerStateComponent>(player1, playerStateComp);

	InputManager& inputManager = InputManager::GetInstance();

	inputManager.AddBinding("JumpP1", Binding{ControllerButton::ButtonA, 'C', InputTriggerState::Pressed, GamepadIndex::PlayerOne});
	inputManager.AddCommand("JumpP1", new JumpCommand(player1));

	//Pressed
	inputManager.AddBinding("LeftP1", Binding{ControllerButton::ButtonDPADLeft, VK_LEFT, InputTriggerState::Down, GamepadIndex::PlayerOne});
	inputManager.AddCommand("LeftP1", new MoveLeftCommand(player1));
	inputManager.AddBinding("RightP1", Binding{ControllerButton::ButtonDPADRight, VK_RIGHT, InputTriggerState::Down, GamepadIndex::PlayerOne});
	inputManager.AddCommand("RightP1", new MoveRightCommand(player1));

}

void crupt::BubbleBobbleScene::FixedUpdate(float dt)
{
	m_pPhysicsSystem->PreUpdate(dt);
	m_pCollisionSystem->Update(dt);
	m_pPhysicsSystem->Update(dt);
}

void crupt::BubbleBobbleScene::Update(float dt)
{	
	m_pPlayerStateSystem->Update(dt);
	m_pFPSSystem->Update(m_FpsCounter, dt);
	m_pFPSSystem->SetText(m_FpsCounter, "FPS:" + std::to_string(m_pFPSSystem->GetFPS(m_FpsCounter)));
	m_pTextSystem->Update(dt);
	m_pSpriteSystem->Update(dt);
}


void crupt::BubbleBobbleScene::Render()
{	
	m_pTileMapSystem->Render();
	m_pRenderSystem->Render();
}

