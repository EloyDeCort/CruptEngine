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
		void Reset();
		void SetPlayer1(Entity player);
		void SetPlayer2(Entity player);
		void OnDispatch(const LevelStateComponent& component);

	private:
		Entity m_Player1;
		Entity m_Player2;

		glm::vec2 m_Player1SpawnLoc;
		glm::vec2 m_Player2SpawnLoc;

		int m_NrOfPlayers;
	};
}

