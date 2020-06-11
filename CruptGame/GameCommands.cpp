#include "GameCommands.h"
#include "Components.h"
#include "GameComponents.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"

crupt::JumpCommand::JumpCommand(Entity pReceiver)
	: ICommand{"JumpCommand"}
	, m_pReceiver{pReceiver}
{
	
}

crupt::JumpCommand::~JumpCommand()
{
}

void crupt::JumpCommand::Execute()
{
	JumpComponent jumpComp;
	jumpComp.m_Target = m_pReceiver;

	SignalHandler<JumpComponent>::GetInstance().Publish(jumpComp);
}

crupt::MoveLeftCommand::MoveLeftCommand(Entity pReceiver)
	: ICommand{"MoveLeftCommand"}
	, m_pReceiver{pReceiver}
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	m_pState = &coordinator->GetComponent<PlayerStateComponent>(m_pReceiver);
}

crupt::MoveLeftCommand::~MoveLeftCommand()
{
}

void crupt::MoveLeftCommand::Execute()
{
	MoveComponent moveComp;
	moveComp.m_Target = m_pReceiver;
	moveComp.m_Speed = 200.f;
	moveComp.m_xDirection = -1.f;
		
	m_pState->m_AnimationState = PlayerAnimState::RUNNING;
	SignalHandler<MoveComponent>::GetInstance().Publish(moveComp);
}
	
crupt::MoveRightCommand::MoveRightCommand(Entity pReceiver)
	: ICommand{"MoveRightCommand"}
	, m_pReceiver{pReceiver}
{	
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	m_pState = &coordinator->GetComponent<PlayerStateComponent>(m_pReceiver);
}

crupt::MoveRightCommand::~MoveRightCommand()
{
}

void crupt::MoveRightCommand::Execute()
{
	MoveComponent moveComp;

	moveComp.m_Target = m_pReceiver;
	moveComp.m_Speed = 200.f;
	moveComp.m_xDirection = 1.f;

	m_pState->m_AnimationState = PlayerAnimState::RUNNING;

	SignalHandler<MoveComponent>::GetInstance().Publish(moveComp);
}