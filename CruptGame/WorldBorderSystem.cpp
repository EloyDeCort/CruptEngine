#include "CruptEnginePCH.h"
#include "WorldBorderSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "GameComponents.h"

crupt::WorldBorderSystem::~WorldBorderSystem()
{
}

void crupt::WorldBorderSystem::Init()
{
	m_MaxY = Settings::windowHeight;
	m_MinY = 50.f;
}

void crupt::WorldBorderSystem::PreUpdate(float )
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	for (Entity entity : m_Entities)
	{
		TransformComponent& transComp = coordinator->GetComponent<TransformComponent>(entity);

		if(transComp.position.y > m_MaxY)
		{
			transComp.position.y = m_MinY;
		}
		else if(transComp.position.y < m_MinY)
		{
			transComp.position.y = m_MaxY;
		}
	}
}
