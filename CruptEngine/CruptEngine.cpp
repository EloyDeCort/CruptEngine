#include "CruptEnginePCH.h"
#include "CruptEngine.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "Scene.h"
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
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	
}

/**
 * Code constructing the scene world starts here
 */
void crupt::CruptEngine::LoadGame()
{
	//Scene& scene = SceneManager::GetInstance().CreateScene("Demo");
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	Entity background = pCoordinator.CreateEntity();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};
	pCoordinator.AddComponent<RenderableComponent>(background, RenderableComponent{ResourceManager::GetInstance().LoadTexture("background.jpg",renderer)});
	pCoordinator.AddComponent<TransformComponent>(background, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	Entity logo = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(logo, RenderableComponent{ResourceManager::GetInstance().LoadTexture("logo.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(logo, TransformComponent{glm::vec3(216.f,180.f,0.f)});

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
}

void crupt::CruptEngine::Cleanup()
{
	m_pRenderSystem->Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void crupt::CruptEngine::Run()
{
	Initialize();
	
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	//Setting up coordinator
	pCoordinator.Initialize();

	//Register Components
	RegisterComponents();

	//Register the systems
	m_pRenderSystem = pCoordinator.RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<RenderableComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		pCoordinator.SetSystemSignature<RenderSystem>(signature);
	}
	m_pRenderSystem->Init(m_Window);

	m_pTextSystem = pCoordinator.RegisterSystem<TextSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<RenderableComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<TextComponent>());
		pCoordinator.SetSystemSignature<TextSystem>(signature);
	}
	m_pTextSystem->Init(m_pRenderSystem->GetSDLRenderer());

	m_pFPSSystem = pCoordinator.RegisterSystem<FPSSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<RenderableComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<TextComponent>());
		signature.set(pCoordinator.GetComponentType<FPSComponent>());
		pCoordinator.SetSystemSignature<FPSSystem>(signature);
	}

	LoadGame();
	{
		//crupt::SceneManager& sceneManager = SceneManager::GetInstance();
		crupt::InputManager& input = InputManager::GetInstance();

		bool doContinue = true;
		std::chrono::steady_clock::time_point lastTime = high_resolution_clock::now();
		while (doContinue)
		{
			const std::chrono::steady_clock::time_point currentTime = high_resolution_clock::now();

			float dt = std::chrono::duration<float>(currentTime - lastTime).count();

			doContinue = input.ProcessInput();

			m_pFPSSystem->SetText(m_FpsCounter, "FPS: " + std::to_string(m_pFPSSystem->GetFPS(m_FpsCounter)));

			m_pTextSystem->Update(dt);

			m_pFPSSystem->Update(m_FpsCounter, dt);

			m_pRenderSystem->Update(dt);
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}


void crupt::CruptEngine::RegisterComponents()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.RegisterComponent<RenderableComponent>();
	pCoordinator.RegisterComponent<TransformComponent>();
	pCoordinator.RegisterComponent<TextComponent>();
	pCoordinator.RegisterComponent<FPSComponent>();
}