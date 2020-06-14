#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace crupt
{
	//Used for applying velocity and force
	struct MovePhysicsComponent
	{
		glm::vec2 velocity{0.f,0.f};
		glm::vec2 force{0.f,0.f};
	};
}