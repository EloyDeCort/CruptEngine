#pragma once
#include "ICommand.h"
#include "GameComponents.h"

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

	class MoveRightCommand : public ICommand
	{
	public:
		MoveRightCommand(Entity pReceiver);	
		virtual ~MoveRightCommand();

		virtual void Execute() override;
	private:
		Entity m_pReceiver;
	};

	
	class SpawnBubbleCommand : public ICommand
	{
	public:
		SpawnBubbleCommand(Entity pPlayer);	
		virtual ~SpawnBubbleCommand();

		virtual void Execute() override;
	private:
		Entity m_pPlayer;
		float m_Offset;
		PlayerStateComponent* m_pState;
	};
	
}