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
		void OnDispatch(MoveComponent component);

	private:
		float m_Time;
		int m_FpsCounter;
	};
}

