#pragma once
#include "System.h"

namespace crupt
{
	class JumpSystem : public System
	{
	public:
		virtual ~JumpSystem();
		void Init();
		void Update(float dt);

	private:
		float m_Time;
		int m_FpsCounter;
	};
}

