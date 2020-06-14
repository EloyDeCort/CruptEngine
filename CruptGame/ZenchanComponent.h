#pragma once

namespace crupt
{
	//Used for zenchan, abilities, walking speed, ...
	struct ZenchanComponent
	{
		Entity player1;
		Entity player2;
		bool coOp = false;

		float movSpeed = 180.f;

		float jumpOffset = 50.f;
		float maxDistanceOffset = 150.f;
		//Gets changed on wall hit
		bool shouldDie = false;
		bool flipped = false;
	};
}