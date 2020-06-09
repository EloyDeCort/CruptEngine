#include "CruptEnginePCH.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "ICommand.h"
#include "SignalHandler.h"
#include "Components.h"

crupt::ICommand::~ICommand()
{
}

const std::string& crupt::ICommand::GetName()
{
	return m_Name;
}

template<typename T>
inline void crupt::ICommand::Dispatch(T signalComponent)
{
	std::cout << "SIGNAL DISPATCHED" << std::endl;
	SignalHandler<T>::GetInstance().Publish(signalComponent);
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
	JumpComponent jumpComp;
	jumpComp.m_Target = m_pReceiver;
	jumpComp.m_JumpHeight = 5.f;

	Dispatch(jumpComp);
}

