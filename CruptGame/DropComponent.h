#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace crupt
{
	enum class DropType
	{
		MELON,
		FRIES
	};

	//Used as simple tag component and transfered as a entity component
	struct DropComponent
	{
		DropType type;
		int score;
		glm::vec2 pos = {0.f,0.f};

		float movTime = 0.1f;
		float totalTime = 0.f;

		bool destroy = false;
	};
}