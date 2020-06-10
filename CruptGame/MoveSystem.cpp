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
	TransformComponent& transform = coordinator->GetComponent<TransformComponent>(component.m_Target);
	//Jump (Temp)
	transform;
	transform.position.x += component.m_xDirection * component.m_Speed;

	if(component.m_xDirection < 0.f)
	{
		std::cout << "[LOG]: MOVING LEFT" << std::endl;
	}
	else
	{
		std::cout << "[LOG]: MOVING RIGHT" << std::endl;
	}
}
