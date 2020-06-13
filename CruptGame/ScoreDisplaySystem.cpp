#include "CruptEnginePCH.h"
#include "ScoreDisplaySystem.h"
#include "ResourceManager.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "SignalHandler.h"

crupt::ScoreDisplaySystem::ScoreDisplaySystem()
	: m_Player1Offset{80.f,35.f}
	, m_Player2Offset{500.f,35.f}
{
}

crupt::ScoreDisplaySystem::~ScoreDisplaySystem()
{
}

void crupt::ScoreDisplaySystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
	SignalHandler<ScoreComponent>::GetInstance().AddListener(std::bind(&crupt::ScoreDisplaySystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::ScoreDisplaySystem::SetPlayer1(Entity player, Entity scoreEntity)
{
	m_Player1 = player;
	m_ScorePlayer1 = scoreEntity;
	m_NrOfPlayers++;

}

void crupt::ScoreDisplaySystem::SetPlayer2(Entity player, Entity scoreEntity)
{
	m_Player2 = player;
	m_ScorePlayer2 = scoreEntity;
	m_NrOfPlayers++;
}

void crupt::ScoreDisplaySystem::Render()
{

	
}

void crupt::ScoreDisplaySystem::OnDispatch(ScoreComponent)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	if(m_NrOfPlayers >= 1)
	{
		ScoreComponent& scoreCompP1 = coordinator->GetComponent<ScoreComponent>(m_Player1);
		TextComponent& textCompP1 = coordinator->GetComponent<TextComponent>(m_ScorePlayer1);

		textCompP1.needsUpdate = true;
		textCompP1.text = std::to_string(scoreCompP1.score);
	}
	
	if(m_NrOfPlayers >= 2)
	{
		ScoreComponent& scoreCompP2 = coordinator->GetComponent<ScoreComponent>(m_Player2);
		TextComponent& textCompP2 = coordinator->GetComponent<TextComponent>(m_ScorePlayer2);

		textCompP2.needsUpdate = true;
		textCompP2.text = std::to_string(scoreCompP2.score);
	}
}


