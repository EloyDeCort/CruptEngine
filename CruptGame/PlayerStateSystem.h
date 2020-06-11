#pragma once
#include "System.h"

namespace crupt
{
	class PlayerStateSystem : public System
	{
	public:
		virtual ~PlayerStateSystem();
		void Init();
		void Update(float dt);

	private:
		void ChangeState();
	};
}

