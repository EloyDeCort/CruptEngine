#include "CruptEnginePCH.h"
#include "CruptEngine.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"

//SDL Initialization
#include <SDL.h>

//VLD
//#include "vld.h"

//ECS
#include "Components.h"

using namespace std;
using namespace std::chrono;

void crupt::CruptEngine::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	
	m_Window = SDL_CreateWindow(
		"Programming 4: CruptEngine",
		SDL_WINDOWPOS_CENTERED,	
		SDL_WINDOWPOS_CENTERED,
		Settings::windowWidth,
		Settings::windowHeight,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}	
}


void crupt::CruptEngine::Cleanup()
{
	//m_pRenderSystem->Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void crupt::CruptEngine::Run()
{
	Initialize();
	
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	//Initialize the scenemanger with singleton instance 
	crupt::SceneManager& sceneManager = SceneManager::GetInstance();
	
	//Setting up coordinator
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.Initialize();

	//Register Basic Engine Components
	RegisterComponents();

	//Register Basic Engine Systems
	RegisterSystems();

	//Initialize the game-dependent systems , components,....
	InitGame();

	{
		//crupt::SceneManager& sceneManager = SceneManager::GetInstance();
		crupt::InputManager& input = InputManager::GetInstance();
		input.Init();
		float dt = 0.f;
		float accumulator = 0.f;
		const float physicsTimeStepMS = 1/66.f;
		bool doContinue = true;
		std::chrono::steady_clock::time_point lastTime = high_resolution_clock::now();
		while (doContinue)
		{
			//CLEAR our renderer.
			SDL_RenderClear(m_pRenderSystem->GetSDLRenderer());

			const std::chrono::steady_clock::time_point currentTime = high_resolution_clock::now();
			dt = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			
			//Make sure our deltatime is capped
			if(dt > 0.25f)
			{
				dt = 0.25f;
			}

			//Accumulate deltatime
			accumulator += dt;

			doContinue = input.ProcessInput();

			while(accumulator >= physicsTimeStepMS)
			{
				//This fixedupdate will be called multiple times per render loop to ensure solid physics
				sceneManager.FixedUpdate(physicsTimeStepMS);
				accumulator -= physicsTimeStepMS;
			}

			//Update the currently active scene
			sceneManager.Update(dt);

			//Render out the current scene
			sceneManager.Render();
			
			//display with SDL
			SDL_RenderPresent(m_pRenderSystem->GetSDLRenderer());
		}
	}

	Cleanup();
}


void crupt::CruptEngine::RegisterComponents()
{
	//Registering all Engine-Based components
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.RegisterComponent<RenderableComponent>();
	pCoordinator.RegisterComponent<TransformComponent>();
	pCoordinator.RegisterComponent<TextComponent>();
	pCoordinator.RegisterComponent<FPSComponent>();
	pCoordinator.RegisterComponent<SpriteComponent>();
	pCoordinator.RegisterComponent<MovePhysicsComponent>();
	pCoordinator.RegisterComponent<GravityComponent>();
	pCoordinator.RegisterComponent<TileMapComponent>();
	pCoordinator.RegisterComponent<BoxCollisionComponent>();
	pCoordinator.RegisterComponent<CollisionCallbackComponent>();
}

void crupt::CruptEngine::RegisterSystems()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();

	//Register the Engine-Based Systems
	m_pRenderSystem = pCoordinator.RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<RenderableComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		pCoordinator.SetSystemSignature<RenderSystem>(signature);
	}
	m_pRenderSystem->Init(m_Window);

	TextSystem* m_pTextSystem = pCoordinator.RegisterSystem<TextSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<RenderableComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<TextComponent>());
		pCoordinator.SetSystemSignature<TextSystem>(signature);
	}
	m_pTextSystem->Init(m_pRenderSystem->GetSDLRenderer());

	FPSSystem* m_pFPSSystem = pCoordinator.RegisterSystem<FPSSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<RenderableComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<TextComponent>());
		signature.set(pCoordinator.GetComponentType<FPSComponent>());
		pCoordinator.SetSystemSignature<FPSSystem>(signature);
	}
	m_pFPSSystem->Init(m_pRenderSystem->GetSDLRenderer());

	SpriteSystem* spriteSystem = pCoordinator.RegisterSystem<SpriteSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<SpriteComponent>());
		signature.set(pCoordinator.GetComponentType<RenderableComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		pCoordinator.SetSystemSignature<SpriteSystem>(signature);
	}
	spriteSystem->Init();

	PhysicsSystem* pPhysicsSystem = pCoordinator.RegisterSystem<PhysicsSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<MovePhysicsComponent>());
		signature.set(pCoordinator.GetComponentType<GravityComponent>());
		signature.set(pCoordinator.GetComponentType<BoxCollisionComponent>());
		pCoordinator.SetSystemSignature<PhysicsSystem>(signature);
	}
	pPhysicsSystem->Init();

	TileMapSystem* tileMapSystem = pCoordinator.RegisterSystem<TileMapSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TileMapComponent>());
		pCoordinator.SetSystemSignature<TileMapSystem>(signature);
	}
	tileMapSystem->Init(m_pRenderSystem->GetSDLRenderer());

	CollisionSystem* collisionSystem = pCoordinator.RegisterSystem<CollisionSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<MovePhysicsComponent>());
		signature.set(pCoordinator.GetComponentType<BoxCollisionComponent>());
		pCoordinator.SetSystemSignature<CollisionSystem>(signature);
	}
	collisionSystem->Init(m_pRenderSystem->GetSDLRenderer());
}