#pragma once

namespace crupt
{

	//This component will cut out straight sprites (uniform, only row based) and send it to a render component
	struct SpriteComponent
	{
		unsigned int frameCount = 0;
		unsigned int currentFrame = 0;
		int animationRate = 12;
		float totalTime = 0.f;
		int scaleFactor = 1;
	};
}