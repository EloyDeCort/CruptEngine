#pragma once
#include "System.h"
#include "GameComponents.h"

namespace crupt
{
	class MoveSystem : public System
	{
	public:
		virtual ~MoveSystem();
		void Init();
		void OnDispatch(const MoveComponent& component);

	private:
	};
}

