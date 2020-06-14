#include "CruptEnginePCH.h"
#include "MoveSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"
#include "SceneManager.h"

crupt::MoveSystem::~MoveSystem()
{
}

void crupt::MoveSystem::Init()
{
	SignalHandler<MoveComponent>::GetInstance().AddListener(std::bind(&crupt::MoveSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::MoveSystem::OnDispatch(const MoveComponent& component)
{
	//Moving based on the dispatched component

	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(component.target);
	RenderableComponent& render = coordinator->GetComponent<RenderableComponent>(component.target);
	BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(component.target);

	if(coordinator->HasComponent<PlayerStateComponent>(component.target))
	{
		HealthComponent& healthComp = coordinator->GetComponent<HealthComponent>(component.target);

		if(healthComp.dead)
			return;
	}


	if(component.speed > 0.01f)
	{
		if(component.m_xDirection > 0.f)
		{
			render.flip = false;
		}
		else
		{
			render.flip = true;
		}
	}


	if(boxComp.colDirY == eDirection::DOWN)
	{
		movPhysicsComp.force.x = component.speed * component.m_xDirection;
	}
	else
	{
		movPhysicsComp.force.x = component.speed * 0.8f * component.m_xDirection;
	}
}
