#pragma once
#include "System.h"
#include "GameComponents.h"

namespace crupt
{
	class JumpSystem : public System
	{
	public:
		virtual ~JumpSystem();
		void Init();
		void OnDispatch(JumpComponent component);

	private:
	};
}

