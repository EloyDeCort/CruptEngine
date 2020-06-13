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
		float m_SpitTime;
		float m_MaxSpitTime;
	};
}

