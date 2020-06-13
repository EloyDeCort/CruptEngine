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
		SDL_Rect collisionRect;
		float entryTimeX;
		float entryTimeY;

		eDirection colDirX = eDirection::NONE;
		eDirection colDirY = eDirection::NONE;

		bool ignorePlatforms = false;
		bool ignoreEntities = false;
	};
}