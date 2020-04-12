#include "CruptEnginePCH.h"
#include "Invoker.h"

crupt::Invoker::Invoker(ICommand* moveCommand)
{
	m_CommandMap[moveCommand->GetName()] = moveCommand;
}

void crupt::Invoker::HandleCommand(const std::string& command)
{
	//Check if in map, if it is --> Execute
	std::unordered_map<std::string, ICommand*>::iterator it = m_CommandMap.find(command);
	if(it != m_CommandMap.end())
	{
		it->second->Execute();
	}
}
