#pragma once
#include "TextSystem.h"

namespace crupt
{
	class FPSSystem : public TextSystem
	{
	public:
		virtual ~FPSSystem();
		int GetFPS(Entity& entity) const;
		void Update(Entity entity, float deltaTime);

	private:
		float m_Time;
		int m_FpsCounter;
	};
}

