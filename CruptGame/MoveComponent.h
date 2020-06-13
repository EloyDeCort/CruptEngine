#pragma once

namespace crupt
{
	//SIGNAL COMPONENT
	struct MoveComponent
	{
		Entity target;
		float speed = 0.f;
		float m_xDirection = 0.f;
	};
}