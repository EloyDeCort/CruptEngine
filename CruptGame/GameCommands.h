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
		MoveLeftCommand(Entity pReceiver, bool stop);	
		virtual ~MoveLeftCommand();

		virtual void Execute() override;
	private:
		Entity m_pReceiver;
		bool m_Stop;
	};

	class MoveRightCommand : public ICommand
	{
	public:
		MoveRightCommand(Entity pReceiver, bool stop);	
		virtual ~MoveRightCommand();

		virtual void Execute() override;
	private:
		Entity m_pReceiver;
		bool m_Stop;
	};
	
}