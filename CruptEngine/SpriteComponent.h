#pragma once

namespace crupt
{
	struct SpriteComponent
	{
		unsigned int frameCount = 0;
		unsigned int currentFrame = 0;
		int animationRate = 12;
		float totalTime = 0.f;
		int scaleFactor = 1;
	};
}