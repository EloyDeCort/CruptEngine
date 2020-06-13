#include "CruptEnginePCH.h"
#include "CruptEngine.h"
#include "BubbleBobbleScene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameSystems.h"
#include "GameComponents.h"
#include "GameCommands.h"
#include "SignalHandler.h"
#include <SDL.h>


#include <iostream>

crupt::BubbleBobbleScene::BubbleBobbleScene()
	: GameScene(L"BubbleBobbleScene")
	, m_FpsCounter{}
{

}

void crupt::BubbleBobbleScene::Init()
{
	InitTextures();
	InitSystems();
	InitEntities();

	//Initalize all callbacks based on entity type.
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.GetSystem<CollisionCallbackSystem>()->Init();
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
	m_pBubbleMovementSystem = pCoordinator.GetSystem<BubbleMovementSystem>();
	m_pZenchanMovementSystem = pCoordinator.GetSystem<ZenchanMovementSystem>();
	m_pSpawnEnemySystem = pCoordinator.GetSystem<SpawnEnemySystem>();
	m_pHealthDisplaySystem = pCoordinator.GetSystem<HealthDisplaySystem>();
	m_pScoreDisplaySystem = pCoordinator.GetSystem<ScoreDisplaySystem>();
}

void crupt::BubbleBobbleScene::InitEntities()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};
	//Simple FPS component
	m_pFont = ResourceManager::GetInstance().LoadFont("Bobble.ttf", 16);
	/*m_FpsCounter = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(m_FpsCounter, RenderableComponent{});
	pCoordinator.AddComponent<TransformComponent>(m_FpsCounter, TransformComponent{glm::vec2(90.f,25.f)});
	pCoordinator.AddComponent<TextComponent>(m_FpsCounter, TextComponent{bool{true}, std::string("FPS:"), m_pFont, glm::vec3(255.f, 255.f, 255.f)});
	pCoordinator.AddComponent<FPSComponent>(m_FpsCounter, FPSComponent{});*/

	//Init the map entity
	Entity map = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TileMapComponent>(map, TileMapComponent{});
	//We pass the map to both systems to optimize the find time. This entity won't change.
	m_pTileMapSystem->InitMap(map);
	m_pCollisionSystem->InitMap(map);

	//UI
	Entity ui = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(ui, RenderableComponent{ResourceManager::GetInstance().LoadTexture("UI.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(ui, TransformComponent{glm::vec2(0.f,0.f)});

	
	InitPlayers();
	InitEnemies();
}

void crupt::BubbleBobbleScene::InitPlayers()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};

	{
		//Player 1 
		SpriteComponent spriteComp{};
		spriteComp.animationRate = 12; 
		spriteComp.scaleFactor = 2; 
		Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Player/Bob_Idle.png",renderer);

		m_Player1 = pCoordinator.CreateEntity();
		pCoordinator.AddComponent<SpriteComponent>(m_Player1, spriteComp);
		pCoordinator.AddComponent<HealthComponent>(m_Player1, HealthComponent{});
		pCoordinator.AddComponent<ScoreComponent>(m_Player1, ScoreComponent{0});
		pCoordinator.AddComponent<RenderableComponent>(m_Player1, RenderableComponent{defaultAnim});
		pCoordinator.AddComponent<TransformComponent>(m_Player1, TransformComponent{glm::vec2(75.f,100.f)});
		pCoordinator.AddComponent<MovePhysicsComponent>(m_Player1, MovePhysicsComponent{});
		pCoordinator.AddComponent<GravityComponent>(m_Player1, GravityComponent{});
		pCoordinator.AddComponent<BoxCollisionComponent>(m_Player1, BoxCollisionComponent{0,0,32,32});
		pCoordinator.AddComponent<CollisionCallbackComponent>(m_Player1, CollisionCallbackComponent{});
		PlayerStateComponent playerStateComp{};
		playerStateComp.animationState = PlayerAnimState::IDLE;

		spriteComp.frameCount = 1;
		playerStateComp.pStateSprites.push_back(StateSprite{spriteComp,defaultAnim});
	
		spriteComp.frameCount = 8;
		playerStateComp.pStateSprites.push_back(StateSprite{spriteComp,ResourceManager::GetInstance().LoadTexture("Player/Bob_Walking.png",renderer)});

		spriteComp.frameCount = 1;
		playerStateComp.pStateSprites.push_back(StateSprite{spriteComp,ResourceManager::GetInstance().LoadTexture("Player/Bob_Spit.png",renderer)});

		pCoordinator.AddComponent<PlayerStateComponent>(m_Player1, playerStateComp);

		InputManager& inputManager = InputManager::GetInstance();

		inputManager.AddBinding("JumpP1", Binding{ControllerButton::ButtonA, 'C', InputTriggerState::Pressed, GamepadIndex::PlayerOne});
		inputManager.AddCommand("JumpP1", new JumpCommand(m_Player1));

		inputManager.AddBinding("SpawnBubbleP1", Binding{ControllerButton::ButtonX, 'X', InputTriggerState::Pressed, GamepadIndex::PlayerOne});
		inputManager.AddCommand("SpawnBubbleP1", new SpawnBubbleCommand(m_Player1, PlayerType::PLAYER1));

		//Pressed
		inputManager.AddBinding("LeftP1", Binding{ControllerButton::ButtonDPADLeft, VK_LEFT, InputTriggerState::Down, GamepadIndex::PlayerOne});
		inputManager.AddCommand("LeftP1", new MoveLeftCommand(m_Player1));
		inputManager.AddBinding("RightP1", Binding{ControllerButton::ButtonDPADRight, VK_RIGHT, InputTriggerState::Down, GamepadIndex::PlayerOne});
		inputManager.AddCommand("RightP1", new MoveRightCommand(m_Player1));

		m_pSpawnEnemySystem->SetPlayer1(m_Player1);
		m_pHealthDisplaySystem->SetPlayer1(m_Player1);


		Entity scoreP1 = pCoordinator.CreateEntity();
		pCoordinator.AddComponent<RenderableComponent>(scoreP1, RenderableComponent{});
		pCoordinator.AddComponent<ScoreComponent>(scoreP1, ScoreComponent{});
		pCoordinator.AddComponent<TransformComponent>(scoreP1, TransformComponent{glm::vec2(90.f,25.f)});
		pCoordinator.AddComponent<TextComponent>(scoreP1, TextComponent{bool{true}, std::string("0"), m_pFont, glm::vec3(255.f, 255.f, 255.f)});

		m_pScoreDisplaySystem->SetPlayer1(m_Player1, scoreP1);
	}

	//{
	//	//Player 2 
	//	SpriteComponent spriteComp{};
	//	spriteComp.animationRate = 12; 
	//	spriteComp.scaleFactor = 2; 
	//	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Player/Bub_Idle.png",renderer);

	//	m_Player2 = pCoordinator.CreateEntity();
	//	pCoordinator.AddComponent<SpriteComponent>(m_Player2, spriteComp);
	//	pCoordinator.AddComponent<HealthComponent>(m_Player2, HealthComponent{});
	//	pCoordinator.AddComponent<RenderableComponent>(m_Player2, RenderableComponent{defaultAnim});
	//	pCoordinator.AddComponent<TransformComponent>(m_Player2, TransformComponent{glm::vec2(100.f,100.f)});
	//	pCoordinator.AddComponent<MovePhysicsComponent>(m_Player2, MovePhysicsComponent{});
	//	pCoordinator.AddComponent<GravityComponent>(m_Player2, GravityComponent{});
	//	pCoordinator.AddComponent<BoxCollisionComponent>(m_Player2, BoxCollisionComponent{0,0,32,32});
	//	pCoordinator.AddComponent<CollisionCallbackComponent>(m_Player2, CollisionCallbackComponent{});
	//	PlayerStateComponent playerStateComp{};
	//	playerStateComp.animationState = PlayerAnimState::IDLE;

	//	spriteComp.frameCount = 1;
	//	playerStateComp.pStateSprites.push_back(StateSprite{spriteComp,defaultAnim});
	//
	//	spriteComp.frameCount = 8;
	//	playerStateComp.pStateSprites.push_back(StateSprite{spriteComp,ResourceManager::GetInstance().LoadTexture("Player/Bub_Walking.png",renderer)});

	//	spriteComp.frameCount = 1;
	//	playerStateComp.pStateSprites.push_back(StateSprite{spriteComp,ResourceManager::GetInstance().LoadTexture("Player/Bub_Spit.png",renderer)});

	//	pCoordinator.AddComponent<PlayerStateComponent>(m_Player2, playerStateComp);

	//	InputManager& inputManager = InputManager::GetInstance();

	//	inputManager.AddBinding("JumpP2", Binding{ControllerButton::ButtonA, 'C', InputTriggerState::Pressed, GamepadIndex::PlayerTwo});
	//	inputManager.AddCommand("JumpP2", new JumpCommand(m_Player2));

	//	inputManager.AddBinding("SpawnBubbleP2", Binding{ControllerButton::ButtonX, 'X', InputTriggerState::Pressed, GamepadIndex::PlayerTwo});
	//	inputManager.AddCommand("SpawnBubbleP2", new SpawnBubbleCommand(m_Player2, PlayerType::PLAYER2));

	//	//Pressed
	//	inputManager.AddBinding("LeftP2", Binding{ControllerButton::ButtonDPADLeft, VK_LEFT, InputTriggerState::Down, GamepadIndex::PlayerTwo});
	//	inputManager.AddCommand("LeftP2", new MoveLeftCommand(m_Player2));
	//	inputManager.AddBinding("RightP2", Binding{ControllerButton::ButtonDPADRight, VK_RIGHT, InputTriggerState::Down, GamepadIndex::PlayerTwo});
	//	inputManager.AddCommand("RightP2", new MoveRightCommand(m_Player2));

	//	//m_pSpawnEnemySystem->SetPlayer1(m_Player1);
	//	m_pHealthDisplaySystem->SetPlayer2(m_Player2);
	//}

}

void crupt::BubbleBobbleScene::InitEnemies()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};

	//Enemy
	SpriteComponent spriteComp{};
	spriteComp.animationRate = 12; 
	spriteComp.scaleFactor = 2; 
	spriteComp.frameCount = 8; 
	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Enemies/ZenChan_Walking.png",renderer);

	Entity zenchanEnemy1 = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<SpriteComponent>(zenchanEnemy1, spriteComp);
	pCoordinator.AddComponent<EnemyComponent>(zenchanEnemy1, EnemyComponent{});
	pCoordinator.AddComponent<RenderableComponent>(zenchanEnemy1, RenderableComponent{defaultAnim});
	pCoordinator.AddComponent<TransformComponent>(zenchanEnemy1, TransformComponent{glm::vec2(Settings::windowWidth/2.f,100.f)});
	pCoordinator.AddComponent<MovePhysicsComponent>(zenchanEnemy1, MovePhysicsComponent{});
	pCoordinator.AddComponent<GravityComponent>(zenchanEnemy1, GravityComponent{});
	pCoordinator.AddComponent<BoxCollisionComponent>(zenchanEnemy1, BoxCollisionComponent{0,0,32,32});
	pCoordinator.AddComponent<CollisionCallbackComponent>(zenchanEnemy1, CollisionCallbackComponent{});
	ZenchanComponent maitaComp = ZenchanComponent{};
	maitaComp.player1 = m_Player1;
	pCoordinator.AddComponent<ZenchanComponent>(zenchanEnemy1, maitaComp);
}

void crupt::BubbleBobbleScene::InitTextures()
{
}

void crupt::BubbleBobbleScene::FixedUpdate(float dt)
{
	m_pZenchanMovementSystem->PreUpdate(dt);
	m_pBubbleMovementSystem->PreUpdate(dt);
	m_pPhysicsSystem->PreUpdate(dt);
	m_pCollisionSystem->Update(dt);
	m_pBubbleMovementSystem->Update(dt);
	m_pPhysicsSystem->Update(dt);
}

void crupt::BubbleBobbleScene::Update(float dt)
{	
	m_pPlayerStateSystem->Update(dt);
	//m_pFPSSystem->Update(m_FpsCounter, dt);
	//m_pFPSSystem->SetText(m_FpsCounter, "FPS:" + std::to_string(m_pFPSSystem->GetFPS(m_FpsCounter)));
	m_pTextSystem->Update(dt);
	m_pSpriteSystem->Update(dt);
}


void crupt::BubbleBobbleScene::Render()
{	
	m_pTileMapSystem->Render();
	m_pRenderSystem->Render();
	m_pHealthDisplaySystem->Render();
}

