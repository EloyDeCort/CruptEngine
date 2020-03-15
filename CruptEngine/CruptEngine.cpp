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
void crupt::CruptEngine::LoadGame() const
{
	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	//GameObject* pBackground{new GameObject()};
	//pBackground->SetTexture("background.jpg");
	//scene.Add(pBackground);

	//GameObject* pLogo{new GameObject()};
	//pLogo->SetTexture("logo.png");
	//pLogo->SetPosition(216, 180);
	//scene.Add(pLogo);

	//crupt::Font* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//TextObject* text = new TextObject("Programming 4 Assignment", font);
	//text->SetPosition(80, 20);
	//scene.Add(text);
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

	LoadGame();

	{
		crupt::Renderer& renderer = Renderer::GetInstance();
		crupt::SceneManager& sceneManager = SceneManager::GetInstance();
		crupt::InputManager& input = InputManager::GetInstance();

		bool doContinue = true;
		while (doContinue)
		{
			const std::chrono::steady_clock::time_point currentTime = high_resolution_clock::now();
			
			doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();
			
			std::chrono::duration<float> sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
