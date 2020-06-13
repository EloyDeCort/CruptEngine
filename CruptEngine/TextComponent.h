#pragma once
#include "Font.h"
#include <string>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)

namespace crupt
{
	struct TextComponent
	{
		bool needsUpdate;
		std::string text;
		Font* pFont;
		glm::vec3 color;
	};
}