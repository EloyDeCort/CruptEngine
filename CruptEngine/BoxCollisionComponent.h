#pragma once

struct SDL_Rect;
namespace crupt
{
	enum class eDirection
	{
		NONE,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	struct BoxCollisionComponent
	{
		SDL_Rect m_CollisionRect;
		float m_EntryTimeX;
		float m_EntryTimeY;

		eDirection m_ColDirX = eDirection::NONE;
		eDirection m_ColDirY = eDirection::NONE;
	};
}