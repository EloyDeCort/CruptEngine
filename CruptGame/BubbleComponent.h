#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)


namespace crupt
{
	//SIGNAL COMPONENT
	struct BubbleComponent
	{
		glm::vec2 position;
		float totalTime;
	};
}