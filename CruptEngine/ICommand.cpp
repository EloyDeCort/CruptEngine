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