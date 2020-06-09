#pragma once
#include "EntityManager.h"
#include "ECSTypes.h"

namespace crupt
{
	class ICommand
	{
	public:
		ICommand(const std::string& name): m_Name{name}{};
		virtual ~ICommand();

		virtual void Execute() = 0;
		const std::string& GetName();

	protected:
		std::string m_Name;
	};
}
