#include "CruptEnginePCH.h"
#include "Invoker.h"
using namespace crupt;

Invoker::Invoker()
	: m_CommandBuffer{}
{
	
}

Invoker::~Invoker()
{
	
}

void Invoker::AddCommand(ICommand* command)
{
	m_CommandBuffer.push(command);
}

//We are using a command buffer, this works with a queue that will pop a command per frame. Thus allowing for per frame execution.
void crupt::Invoker::Update()
{
	if(m_CommandBuffer.size() > 0)
	{
		ICommand* command = m_CommandBuffer.front();
		command->Execute();
		m_CommandBuffer.pop();
		delete command;
		command = nullptr;
	}
}
