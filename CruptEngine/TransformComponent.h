#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

//Almost every entity needs this component to ensure a common component that every entity has. Also the core of position in screenspace.
struct TransformComponent
{
	glm::vec2 position;
};