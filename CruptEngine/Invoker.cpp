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
