#pragma once

namespace crupt
{
	struct BoulderComponent
	{
		float speed = 120.f;
		bool shouldDestroy = false;
		bool flipped = false;

		bool fromPlayer = false;
		Entity player2;

		float totalTime = 0.f;
		float maxAliveTime = 3.f;

		glm::vec2 pos {0.f, 0.f};
	};
}