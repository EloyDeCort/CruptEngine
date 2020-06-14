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
		void Reset();
		void Update(float dt);
		void OnDispatch(const LevelStateComponent& component);
		void SetPlayer1(Entity player);
		void SetPlayer2(Entity player);

	private:
		Entity m_MapEntity;
		Entity m_Player1;
		Entity m_Player2;
		float m_TotalTime;
		float m_TransitionTime;

		int m_NrOfPlayers;
	};
}

