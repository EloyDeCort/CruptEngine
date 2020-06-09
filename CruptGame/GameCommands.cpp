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

crupt::MoveLeftCommand::MoveLeftCommand(Entity pReceiver)
	: ICommand{"MoveLeftCommand"}
	, m_pReceiver{pReceiver}
{
	
}

crupt::MoveLeftCommand::~MoveLeftCommand()
{
}

void crupt::MoveLeftCommand::Execute()
{
	JumpComponent jumpComp;
	jumpComp.m_Target = m_pReceiver;
	jumpComp.m_JumpHeight = 5.f;

	SignalHandler<JumpComponent>::GetInstance().Publish(jumpComp);
}