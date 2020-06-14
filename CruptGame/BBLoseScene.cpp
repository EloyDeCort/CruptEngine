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
	
	InputManager& inputManager = InputManager::GetInstance();
	inputManager.AddBinding("ToMainMenu", Binding{ControllerButton::ButtonA, VK_RETURN, InputTriggerState::Pressed, GamepadIndex::PlayerOne});
	inputManager.AddCommand("ToMainMenu", new ToMainMenuCommand());
}

void crupt::BBLoseScene::Update(float dt)
{
	m_pTextSystem->Update(dt);
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

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	m_pTextSystem = pCoordinator.GetSystem<TextSystem>();
	m_pFont = ResourceManager::GetInstance().LoadFont("Bobble.ttf", 30);

	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};
	//UI
	Entity menu = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(menu, RenderableComponent{ResourceManager::GetInstance().LoadTexture("LoseScreen.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(menu, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	//Score entities
	Entity scoreP1 = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(scoreP1, RenderableComponent{});
	pCoordinator.AddComponent<ScoreComponent>(scoreP1, ScoreComponent{});
	pCoordinator.AddComponent<TransformComponent>(scoreP1, TransformComponent{glm::vec2(Settings::windowWidth/2.f - 110.f,210.f)});
	pCoordinator.AddComponent<TextComponent>(scoreP1, TextComponent{bool{true}, std::to_string(m_ScoreP1), m_pFont, glm::vec3(0.f, 255.f, 0.f)});

	Entity scoreP2 = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(scoreP2, RenderableComponent{});
	pCoordinator.AddComponent<ScoreComponent>(scoreP2, ScoreComponent{});
	pCoordinator.AddComponent<TransformComponent>(scoreP2, TransformComponent{glm::vec2(Settings::windowWidth/2.f - 110.f, 300.f)});
	pCoordinator.AddComponent<TextComponent>(scoreP2, TextComponent{bool{true}, std::to_string(m_ScoreP2), m_pFont, glm::vec3(0.f, 170.f, 255.f)});

}

void crupt::BBLoseScene::SceneUnloaded()
{
	InputManager& inputManager = InputManager::GetInstance();
	inputManager.Reset();
	m_pTextSystem->Reset();

	FMOD_RESULT fmodResult;
	fmodResult = m_pChannelGroup->stop();
	SoundManager::GetInstance().ErrorCheck(fmodResult);

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	pCoordinator.DestroyAllEntities();
}

void crupt::BBLoseScene::SetScoreP1(int score)
{
	m_ScoreP1 = score;
}

void crupt::BBLoseScene::SetScoreP2(int score)
{
	m_ScoreP2 = score;
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

