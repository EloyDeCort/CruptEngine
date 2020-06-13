#pragma once
#include "System.h"
#include "GameComponents.h"

namespace crupt
{
	class LevelSpawnSystem : public System
	{
	public:
		virtual ~LevelSpawnSystem();
		void Init();
		void OnDispatch(const LevelStateComponent& component);

	private:
	};
}

