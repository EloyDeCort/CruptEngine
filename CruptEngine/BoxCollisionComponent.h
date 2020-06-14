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


	//Used for basic collision with the collison system
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