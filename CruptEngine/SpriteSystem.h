#pragma once
#include "System.h"
#include "ECSCoordinator.h"

namespace crupt
{
	class SpriteSystem : public System
	{
	public:
		virtual ~SpriteSystem();
		void Init();
		void Update(float dt);

	private:
	};
}

