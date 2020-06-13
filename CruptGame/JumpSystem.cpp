#include "CruptEnginePCH.h"
#include "JumpSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"

crupt::JumpSystem::~JumpSystem()
{
}

void crupt::JumpSystem::Init()
{
	SignalHandler<JumpComponent>::GetInstance().AddListener(std::bind(&crupt::JumpSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::JumpSystem::OnDispatch(const JumpComponent& component)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(component.target);
	BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(component.target);

	if(boxComp.colDirY == eDirection::DOWN)
	{
		movPhysicsComp.force.y = component.jumpHeight;
	}
	
}
