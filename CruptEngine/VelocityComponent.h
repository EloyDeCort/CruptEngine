#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace crupt
{
	struct MovePhysicsComponent
	{
		glm::vec2 m_Velocity{0.f,0.f};
		glm::vec2 m_Force{0.f,0.f};
	};
}