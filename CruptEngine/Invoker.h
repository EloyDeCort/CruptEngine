#pragma once
#include "ICommand.h"
#include <queue>
#include "Singleton.h"

namespace crupt
{
	class Invoker : public Singleton<Invoker>
	{
	public:
		Invoker();
		~Invoker();
		void AddCommand(ICommand* command);
		void Update();
	private:
		std::queue<ICommand*> m_CommandBuffer;
	};
}

