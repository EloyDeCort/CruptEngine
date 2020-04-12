#pragma once
#include "ICommand.h"
#include <unordered_map>

namespace crupt
{
	class Invoker
	{
	public:
		Invoker(ICommand* moveCommand);
		void HandleCommand(const std::string& command);

	private:
		std::unordered_map<std::string, ICommand*> m_CommandMap;
	};
}

