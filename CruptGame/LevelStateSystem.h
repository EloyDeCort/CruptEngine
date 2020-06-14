#pragma once
#include "System.h"
#include "GameComponents.h"

namespace crupt
{
	class LevelStateSystem : public System
	{
	public:
		virtual ~LevelStateSystem();
		void Init();
		void SetMap(Entity mapEntity);
		void Update(float dt);
		void OnDispatch(const LevelStateComponent& component);

	private:
		Entity m_MapEntity;
		float m_TotalTime;
		float m_TransitionTime;
	};
}

