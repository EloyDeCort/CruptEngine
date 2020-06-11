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

void crupt::JumpSystem::OnDispatch(JumpComponent component)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(component.m_Target);
	BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(component.m_Target);

	if(boxComp.m_IsGrounded)
	{
		movPhysicsComp.m_Force.y = component.m_JumpHeight;
		boxComp.m_IsGrounded = false;
		std::cout << "[LOG]: JUMPING" << std::endl;
	}
	else
	{
		std::cout << "[ERROR]: CAAAAAAAAAAAAAAAAAAAANT JUMP" << std::endl;
	}

}
