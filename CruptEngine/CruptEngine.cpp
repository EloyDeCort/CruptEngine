#include "CruptEnginePCH.h"
#include "CruptEngine.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "TextObject.h"
#include "GameObject.h"
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

	Renderer::GetInstance().Init(m_Window);
	
}

/**
 * Code constructing the scene world starts here
 */
void crupt::CruptEngine::LoadGame()
{
	//Scene& scene = SceneManager::GetInstance().CreateScene("Demo");

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	Entity background = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TextureComponent>(background, TextureComponent{ResourceManager::GetInstance().LoadTexture("background.jpg")});
	pCoordinator.AddComponent<TransformComponent>(background, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	Entity logo = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TextureComponent>(logo, TextureComponent{ResourceManager::GetInstance().LoadTexture("logo.png")});
	pCoordinator.AddComponent<TransformComponent>(logo, TransformComponent{glm::vec3(216.f,180.f,0.f)});

	crupt::Font* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	Entity text = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TextureComponent>(text, TextureComponent{});
	pCoordinator.AddComponent<TransformComponent>(text, TransformComponent{glm::vec3(80.f,20.f,0.f)});
	pCoordinator.AddComponent<TextComponent>(text, TextComponent{bool{true}, std::string("Programming 4 Assignment"), font, glm::vec3(255.f, 0.f, 0.f)});

	crupt::Font* fontFps = ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	m_FpsCounter = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TextureComponent>(m_FpsCounter, TextureComponent{});
	pCoordinator.AddComponent<TransformComponent>(m_FpsCounter, TransformComponent{glm::vec3(0.f,0.f,0.f)});
	pCoordinator.AddComponent<TextComponent>(m_FpsCounter, TextComponent{bool{true}, std::string("FPS: "), fontFps, glm::vec3(255.f, 255.f, 0.f)});
	pCoordinator.AddComponent<FPSComponent>(m_FpsCounter, FPSComponent{});
}

void crupt::CruptEngine::Cleanup()
{
	Renderer::GetInstance().Destroy();
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
	m_pGraphicsSystem = pCoordinator.RegisterSystem<GraphicsSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TextureComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		pCoordinator.SetSystemSignature<GraphicsSystem>(signature);
	}

	m_pTextSystem = pCoordinator.RegisterSystem<TextSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TextureComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<TextComponent>());
		pCoordinator.SetSystemSignature<TextSystem>(signature);
	}

	m_pFPSSystem = pCoordinator.RegisterSystem<FPSSystem>();
	{
		Signature signature;
		signature.set(pCoordinator.GetComponentType<TextureComponent>());
		signature.set(pCoordinator.GetComponentType<TransformComponent>());
		signature.set(pCoordinator.GetComponentType<TextComponent>());
		signature.set(pCoordinator.GetComponentType<FPSComponent>());
		pCoordinator.SetSystemSignature<FPSSystem>(signature);
	}



	LoadGame();
	{
		crupt::Renderer& renderer = Renderer::GetInstance();
		//crupt::SceneManager& sceneManager = SceneManager::GetInstance();
		crupt::InputManager& input = InputManager::GetInstance();

		bool doContinue = true;
		std::chrono::steady_clock::time_point lastTime = high_resolution_clock::now();
		while (doContinue)
		{
			const std::chrono::steady_clock::time_point currentTime = high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			deltaTime;
			doContinue = input.ProcessInput();
			//sceneManager.Update();
			m_pFPSSystem->UpdateFPS(m_FpsCounter, deltaTime);
			m_pFPSSystem->SetText(m_FpsCounter, "FPS: " + std::to_string(m_pFPSSystem->GetFPS(m_FpsCounter)));
			m_pTextSystem->Update();
			m_pFPSSystem->Update();
			renderer.Render(m_pGraphicsSystem, m_pTextSystem, m_pFPSSystem);
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}


void crupt::CruptEngine::RegisterComponents()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.RegisterComponent<TextureComponent>();
	pCoordinator.RegisterComponent<TransformComponent>();
	pCoordinator.RegisterComponent<TextComponent>();
	pCoordinator.RegisterComponent<FPSComponent>();
}