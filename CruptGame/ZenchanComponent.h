#pragma once

namespace crupt
{
	struct ZenchanComponent
	{
		Entity player1;
		Entity player2;
		bool coOp = false;

		float movSpeed = 200.f;

		float jumpOffset = 50.f;
		float maxDistanceOffset = 150.f;
		//Gets changed on wall hit
		bool shouldDie = false;
		bool flipped = false;
	};
}