#pragma once
#include "ICommand.h"

namespace crupt
{
	class JumpCommand : public ICommand
	{
	public:
		JumpCommand(Entity pReceiver);	
		virtual ~JumpCommand();

		virtual void Execute() override;
	private:
		Entity m_pReceiver;
	};

	class MoveLeftCommand : public ICommand
	{
	public:
		MoveLeftCommand(Entity pReceiver);	
		virtual ~MoveLeftCommand();

		virtual void Execute() override;
	private:
		Entity m_pReceiver;
	};
	
}