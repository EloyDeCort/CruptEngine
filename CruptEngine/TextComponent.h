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
		bool m_NeedsUpdate;
		std::string m_Text;
		Font* m_pFont;
		glm::vec3 m_Color;
	};
}