#include "CruptEnginePCH.h"
#include "BBMainMenuScene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameSystems.h"
#include "GameComponents.h"
#include "SceneManager.h"
#include "BubbleBobbleScene.h"
#include "SoundManager.h"
#include "MenuCommands.h"
#include <SDL.h>


#include <iostream>

crupt::BBMainMenuScene::BBMainMenuScene()
	: GameScene(L"BBMainMenuScene")
	,m_GameScene{nullptr}
{
	InitAudio();
}

void crupt::BBMainMenuScene::Init()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};

	//UI
	Entity menu = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(menu, RenderableComponent{ResourceManager::GetInstance().LoadTexture("MainMenu.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(menu, TransformComponent{glm::vec3(0.f,0.f,0.f)});


	InputManager& inputManager = InputManager::GetInstance();

	inputManager.AddBinding("StartSingle", Binding{ControllerButton::ButtonA, '1', InputTriggerState::Pressed, GamepadIndex::PlayerTwo});
	inputManager.AddCommand("StartSingle", new SingleCommand());

}

void crupt::BBMainMenuScene::Update(float dt)
{
	m_TotalTime += dt;
	//crupt::SceneManager& sceneManager = SceneManager::GetInstance();
	//if(m_TotalTime >= 2.f)
	//{
	//	sceneManager.SetActiveScene(L"BubbleBobbleScene");
	//	m_TotalTime = 0.f;
	//}
}

void crupt::BBMainMenuScene::FixedUpdate(float)
{

}

void crupt::BBMainMenuScene::Render()
{	
	m_pRenderSystem->Render();
}

void crupt::BBMainMenuScene::SceneLoaded()
{
	//Play Sound
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	FMOD_RESULT fmodResult;
	fmodResult = pFmodSystem->playSound(m_pBGMusic, m_pChannelGroup, false , &m_pChannel);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
}

void crupt::BBMainMenuScene::SceneUnloaded()
{
	InputManager& inputManager = InputManager::GetInstance();
	inputManager.Reset();

	FMOD_RESULT fmodResult;
	fmodResult = m_pChannelGroup->stop();
	SoundManager::GetInstance().ErrorCheck(fmodResult);

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.DestroyAllEntities();
}

void crupt::BBMainMenuScene::InitAudio()
{
	//INIT Audio
	FMOD_RESULT fmodResult;
	auto pFmodSystem = SoundManager::GetInstance().GetSystem();
	fmodResult = pFmodSystem->createStream("../Data/Sound/BG/StartScreen.ogg", FMOD_DEFAULT, 0, &m_pBGMusic);
	SoundManager::GetInstance().ErrorCheck(fmodResult);
	m_pBGMusic->setMode(FMOD_LOOP_NORMAL);

	pFmodSystem->createChannelGroup("BG",&m_pChannelGroup);
}

