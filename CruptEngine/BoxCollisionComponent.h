#pragma once

struct SDL_Rect;
namespace crupt
{
	enum class eDirection
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	struct BoxCollisionComponent
	{
		SDL_Rect m_CollisionRect;
		bool m_Colliding;
		float m_EntryTime;

		bool m_IsGrounded = false;

		eDirection m_CollisionDir = eDirection::NONE;
	};
}