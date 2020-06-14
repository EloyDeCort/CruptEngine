#pragma once
#include "BubbleComponent.h"

namespace crupt
{
	//Health component used for initiating player health and handling damage.
	struct HealthComponent
	{
		int maxHealth = 4;
		int currentHealth = 4;

		bool dead = false;

		bool gotHit = false;
		float totalTime = 0.f;
		float hitDelay = 1.f;

		//Used only for signal
		PlayerType type = PlayerType::PLAYER1;
	};
}