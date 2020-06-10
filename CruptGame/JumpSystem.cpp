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
	VelocityComponent& velocity = coordinator->GetComponent<VelocityComponent>(component.m_Target);
	//Jump (Temp)
	velocity.m_yVelocity = -component.m_JumpHeight;
	std::cout << "[LOG]: JUMPING" << std::endl;
}
