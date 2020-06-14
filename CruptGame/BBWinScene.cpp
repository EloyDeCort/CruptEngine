#include "CruptEnginePCH.h"
#include "BBWinScene.h"
#include "SoundManager.h"
#include "MenuCommands.h"
#include "InputManager.h"
#include "ResourceManager.h"

#include <iostream>

crupt::BBWinScene::BBWinScene()
	: GameScene(L"BBWinScene")
	, m_ScoreP1{0}
	, m_ScoreP2{0}
{
	InitAudio();
}

void crupt::BBWinScene::Init()
{

	InputManager& inputManager = InputManager::GetInstance();
	inputManager.AddBinding("ToMainMenu", Binding{ControllerButton::ButtonA, VK_RETURN, InputTriggerState::Pressed, GamepadIndex::PlayerOne});
	inputManager.AddCommand("ToMainMenu", new ToMainMenuCommand());
}

void crupt::BBWinScene::Update(float dt)
{
	m_pTextSystem->Update(dt);
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

	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	m_pTextSystem = pCoordinator.GetSystem<TextSystem>();
	m_pFont = ResourceManager::GetInstance().LoadFont("Bobble.ttf", 30);

	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};
	//UI
	Entity menu = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(menu, RenderableComponent{ResourceManager::GetInstance().LoadTexture("WinScreen.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(menu, TransformComponent{glm::vec3(0.f,0.f,0.f)});

	//Initiializing score entities
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

void crupt::BBWinScene::SceneUnloaded()
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

void crupt::BBWinScene::SetScoreP1(int score)
{
	m_ScoreP1 = score;
}

void crupt::BBWinScene::SetScoreP2(int score)
{
	m_ScoreP2 = score;
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

