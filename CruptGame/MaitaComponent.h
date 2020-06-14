#pragma once
#include "PlayerStateComponent.h"
#include <vector>

namespace crupt
{
	enum class MaitaAnimState
	{
		WALKING,
		CHARGING
	};

	struct MaitaComponent
	{
		Entity player1;
		Entity player2;
		bool coOp = false;

		float movSpeed = 75.f;

		float jumpOffset = 50.f;
		float maxDistanceOffset = 150.f;
		float maxBoulderDistanceOffset = 250.f;
		//Gets changed on wall hit
		bool shouldDie = false;
		bool flipped = false;

		float chargeCoolDown = 5.f;
		float chargeTime = 0.5f;
		float totalTime = 5.f;

		MaitaAnimState state = MaitaAnimState::WALKING;
		std::vector<StateSprite> pStateSprites;
	};
}