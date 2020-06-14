#include "CruptEnginePCH.h"
#include "BBLoseScene.h"
#include "SoundManager.h"
#include "MenuCommands.h"
#include "InputManager.h"
#include "ResourceManager.h"

#include <iostream>

crupt::BBLoseScene::BBLoseScene()
	: GameScene(L"BBLoseScene")
{
	InitAudio();
}

void crupt::BBLoseScene::Init()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};

	//UI
	Entity menu = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(menu, RenderableComponent{ResourceManager::GetInstance().LoadTexture("LoseScreen.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(menu, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	InputManager& inputManager = InputManager::GetInstance();
	inputManager.AddBinding("ToMainMenu", Binding{ControllerButton::ButtonA, VK_RETURN, InputTriggerState::Pressed, GamepadIndex::PlayerOne});
	inputManager.AddCommand("ToMainMenu", new ToMainMenuCommand());
}

void crupt::BBLoseScene::Update(float)
{
	//m_TotalTime += dt;
	//crupt::SceneManager& sceneManager = SceneManager::GetInstance();
	//if(m_TotalTime >= 2.f)
	//{
	//	sceneManager.SetActiveScene(L"BubbleBobbleScene");
	//	m_TotalTime = 0.f;
	//}
}

void crupt::BBLoseScene::FixedUpdate(float)
{

}

void crupt::BBLoseScene::Render()
{	
	m_pRenderSystem->Render();
}

void crupt::BBLoseScene::SceneLoaded()
{
	////Play Sound
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	FMOD_RESULT fmodResult;
	fmodResult = pFmodSystem->playSound(m_pBGMusic, m_pChannelGroup, false , &m_pChannel);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
}

void crupt::BBLoseScene::SceneUnloaded()
{
	InputManager& inputManager = InputManager::GetInstance();
	inputManager.Reset();

	FMOD_RESULT fmodResult;
	fmodResult = m_pChannelGroup->stop();
	SoundManager::GetInstance().ErrorCheck(fmodResult);

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.DestroyAllEntities();
}

void crupt::BBLoseScene::InitAudio()
{
	//INIT Audio
	FMOD_RESULT fmodResult;
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	fmodResult = pFmodSystem->createStream("../Data/Sound/SFX/GameOver.wav", FMOD_DEFAULT, 0, &m_pBGMusic);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
	m_pBGMusic->setMode(FMOD_LOOP_OFF);

	pFmodSystem->createChannelGroup("BG",&m_pChannelGroup);
}

