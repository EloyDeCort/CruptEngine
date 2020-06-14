#pragma once
#include "System.h"

namespace crupt
{
	class WorldBorderSystem : public System
	{
	public:
		virtual ~WorldBorderSystem();
		void Init();
		void PreUpdate(float dt);

	private:
		float m_MaxY;
		float m_MinY;
	};
}

