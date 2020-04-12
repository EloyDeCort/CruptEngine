#include "CruptEnginePCH.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "ICommand.h"

crupt::ICommand::~ICommand()
{
}

const std::string& crupt::ICommand::GetName()
{
	return m_Name;
}


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
	ECSCoordinator* pCoordinator = &ECSCoordinator::GetInstance();
	pCoordinator->AddComponent<JumpComponent>(m_pReceiver, JumpComponent{5000.f});
}

