#pragma once

struct SDL_Rect;
namespace crupt
{

	struct BoxCollisionComponent
	{
		SDL_Rect m_CollisionRect;
		bool m_Colliding;
		float m_EntryTime;

		bool m_IsGrounded = false;
	};
}