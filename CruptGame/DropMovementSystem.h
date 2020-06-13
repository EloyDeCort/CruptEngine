#pragma once
#include "System.h"

namespace crupt
{
	class DropMovementSystem : public System
	{
	public:
		virtual ~DropMovementSystem();
		void Init();
		void PreUpdate(float dt);

	private:
	};
}

