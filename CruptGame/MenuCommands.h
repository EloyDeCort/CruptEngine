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
}