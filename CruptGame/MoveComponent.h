#pragma once

namespace crupt
{
	struct MoveComponent
	{
		Entity m_Target;
		float m_Speed = 0.f;
		float m_xDirection = 0.f;
	};
}