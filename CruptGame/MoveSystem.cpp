#include "CruptEnginePCH.h"
#include "MoveSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"

crupt::MoveSystem::~MoveSystem()
{
}

void crupt::MoveSystem::Init()
{
	SignalHandler<MoveComponent>::GetInstance().AddListener(std::bind(&crupt::MoveSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::MoveSystem::OnDispatch(MoveComponent component)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(component.m_Target);
	RenderableComponent& render = coordinator->GetComponent<RenderableComponent>(component.m_Target);

	if(component.m_Speed > 0.01f)
	{
		if(component.m_xDirection > 0.f)
		{
			render.m_Flip = false;
		}
		else
		{
			render.m_Flip = true;
		}
	}

	movPhysicsComp.m_Force.x = component.m_Speed * component.m_xDirection;
}
