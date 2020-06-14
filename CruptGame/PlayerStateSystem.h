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
		void SetPlayer1(Entity player);
		void SetPlayer2(Entity player);
		void Reset();

	private:
		float m_SpitTime;
		float m_MaxSpitTime;

		Entity m_Player1;
		Entity m_Player2;

		int m_NrOfPlayers;
	};
}

