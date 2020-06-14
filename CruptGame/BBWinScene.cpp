#include "CruptEnginePCH.h"
#include "BBWinScene.h"
#include "SoundManager.h"
#include "MenuCommands.h"
#include "InputManager.h"
#include "ResourceManager.h"

#include <iostream>

crupt::BBWinScene::BBWinScene()
	: GameScene(L"BBWinScene")
{
	InitAudio();
}

void crupt::BBWinScene::Init()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};

	//UI
	Entity menu = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(menu, RenderableComponent{ResourceManager::GetInstance().LoadTexture("WinScreen.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(menu, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	InputManager& inputManager = InputManager::GetInstance();
	inputManager.AddBinding("ToMainMenu", Binding{ControllerButton::ButtonA, VK_RETURN, InputTriggerState::Pressed, GamepadIndex::PlayerOne});
	inputManager.AddCommand("ToMainMenu", new ToMainMenuCommand());
}

void crupt::BBWinScene::Update(float)
{
	//m_TotalTime += dt;
	//crupt::SceneManager& sceneManager = SceneManager::GetInstance();
	//if(m_TotalTime >= 2.f)
	//{
	//	sceneManager.SetActiveScene(L"BubbleBobbleScene");
	//	m_TotalTime = 0.f;
	//}
}

void crupt::BBWinScene::FixedUpdate(float)
{

}

void crupt::BBWinScene::Render()
{	
	m_pRenderSystem->Render();
}

void crupt::BBWinScene::SceneLoaded()
{
	////Play Sound
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	FMOD_RESULT fmodResult;
	fmodResult = pFmodSystem->playSound(m_pBGMusic, m_pChannelGroup, false , &m_pChannel);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
}

void crupt::BBWinScene::SceneUnloaded()
{
	InputManager& inputManager = InputManager::GetInstance();
	inputManager.Reset();

	FMOD_RESULT fmodResult;
	fmodResult = m_pChannelGroup->stop();
	SoundManager::GetInstance().ErrorCheck(fmodResult);

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.DestroyAllEntities();
}

void crupt::BBWinScene::InitAudio()
{
	//INIT Audio
	FMOD_RESULT fmodResult;
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	fmodResult = pFmodSystem->createStream("../Data/Sound/SFX/Victory.ogg", FMOD_DEFAULT, 0, &m_pBGMusic);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
	m_pBGMusic->setMode(FMOD_LOOP_OFF);

	pFmodSystem->createChannelGroup("BG",&m_pChannelGroup);
}

