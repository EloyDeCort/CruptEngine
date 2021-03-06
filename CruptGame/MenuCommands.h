#pragma once
#include "ICommand.h"
#include "GameComponents.h"



namespace crupt
{
	class SingleCommand : public ICommand
	{
	public:
		SingleCommand();	
		virtual ~SingleCommand();

		virtual void Execute() override;
	private:
		
	};

	class CoOpCommand : public ICommand
	{
	public:
		CoOpCommand();	
		virtual ~CoOpCommand();

		virtual void Execute() override;
	private:
	};

	class VersusCommand : public ICommand
	{
	public:
		VersusCommand();	
		virtual ~VersusCommand();

		virtual void Execute() override;
	private:
	};

	class ToMainMenuCommand : public ICommand
	{
	public:
		ToMainMenuCommand();	
		virtual ~ToMainMenuCommand();

		virtual void Execute() override;
	private:
		
	};
}