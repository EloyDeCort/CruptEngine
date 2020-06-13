#include "CruptEnginePCH.h"
#include "HealthDisplaySystem.h"
#include "ResourceManager.h"
#include "ECSCoordinator.h"
#include "Components.h"


crupt::HealthDisplaySystem::HealthDisplaySystem()
	: m_pRenderer{nullptr}
	, m_NrOfPlayers{0}
	, m_HealthOffset{16}
	, m_Player1Offset{glm::vec2{10.f, 10.f}}
	, m_Player2Offset{glm::vec2{10.f, 10.f}}
{
}

crupt::HealthDisplaySystem::~HealthDisplaySystem()
{
}

void crupt::HealthDisplaySystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
	m_pHealthP1Texture = ResourceManager::GetInstance().LoadTexture("UI/HealthP1.png",m_pRenderer);
	m_pHealthP2Texture = ResourceManager::GetInstance().LoadTexture("UI/HealthP2.png",m_pRenderer);
}

void crupt::HealthDisplaySystem::SetPlayer1(Entity player)
{
	m_Player1 = player;
	m_NrOfPlayers++;
}

void crupt::HealthDisplaySystem::SetPlayer2(Entity player)
{
	m_Player2 = player;

	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	HealthComponent& healthComp = coordinator->GetComponent<HealthComponent>(m_Player2);

	m_NrOfPlayers++;
	m_Player2Offset = {glm::vec2{Settings::windowWidth - 10.f - (m_HealthOffset * healthComp.maxHealth), 10.f}};
}

void crupt::HealthDisplaySystem::Render()
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	if(m_NrOfPlayers >= 1)
	{
		HealthComponent& healthComp = coordinator->GetComponent<HealthComponent>(m_Player1);

		for(int i{}; i < healthComp.currentHealth; ++i)
		{
			RenderTexture(*m_pHealthP1Texture, m_Player1Offset.x + (m_HealthOffset * i),  m_Player1Offset.y);
		}
	}

	if(m_NrOfPlayers >= 2)
	{
		HealthComponent& healthComp = coordinator->GetComponent<HealthComponent>(m_Player2);

		for(int i{}; i < healthComp.currentHealth; ++i)
		{
			RenderTexture(*m_pHealthP2Texture, m_Player2Offset.x + (m_HealthOffset * i),  m_Player2Offset.y);
		}
	}
}

void crupt::HealthDisplaySystem::RenderTexture(const Texture2D& texture, float x, float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(m_pRenderer, texture.GetSDLTexture(), nullptr, &dst);
}
