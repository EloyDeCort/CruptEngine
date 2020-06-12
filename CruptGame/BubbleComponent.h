#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)


namespace crupt
{
	//SIGNAL COMPONENT
	//Later used as normal component (after spawn)
	struct BubbleComponent
	{
		glm::vec2 position;
		float totalTime = 0.f;
		float xSpeed = 200.f;
		float xSpeedTime = 0.5f;
		bool flipped = false;

		bool shouldPop = false;
	};
}