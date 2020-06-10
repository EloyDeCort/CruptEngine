#include "GameCommands.h"
#include "Components.h"
#include "GameComponents.h"
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
	jumpComp.m_JumpHeight = 5.f;

	SignalHandler<JumpComponent>::GetInstance().Publish(jumpComp);
}

crupt::MoveLeftCommand::MoveLeftCommand(Entity pReceiver, bool stop)
	: ICommand{"MoveLeftCommand"}
	, m_pReceiver{pReceiver}
	, m_Stop{stop}
{
	
}

crupt::MoveLeftCommand::~MoveLeftCommand()
{
}

void crupt::MoveLeftCommand::Execute()
{
	MoveComponent moveComp;
	moveComp.m_Target = m_pReceiver;
	if(!m_Stop)
	{
		moveComp.m_Speed = 0.2f;
		moveComp.m_xDirection = -1.f;
	}
	SignalHandler<MoveComponent>::GetInstance().Publish(moveComp);
}
	
crupt::MoveRightCommand::MoveRightCommand(Entity pReceiver, bool stop)
	: ICommand{"MoveRightCommand"}
	, m_pReceiver{pReceiver}
	, m_Stop{stop}
{	
}

crupt::MoveRightCommand::~MoveRightCommand()
{
}

void crupt::MoveRightCommand::Execute()
{
	MoveComponent moveComp;
	moveComp.m_Target = m_pReceiver;
	if(!m_Stop)
	{
		moveComp.m_Speed = 0.2f;
		moveComp.m_xDirection = 1.f;
	}
	SignalHandler<MoveComponent>::GetInstance().Publish(moveComp);
}