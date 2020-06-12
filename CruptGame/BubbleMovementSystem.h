#pragma once
#include "System.h"
#include "ECSCoordinator.h"

namespace crupt
{
	class BubbleMovementSystem : public System
	{
	public:
		virtual ~BubbleMovementSystem();
		void Init();
		void Update(float dt);

	private:
		
	};
}

