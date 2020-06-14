#pragma once
#include "SpriteComponent.h"
#include "Texture2D.h"
#include "PlayerStateComponent.h"
#include <vector>

namespace crupt
{
	//State of the bubble
	enum class BubbleAnimState
	{
		NORMAL,
		ZENCHAN,
		MAITA
	};

	//Used for updating bubble sprite
	struct BubbleStateComponent
	{
		Entity target;

		BubbleAnimState animationState;
		std::vector<StateSprite> stateSprites;
	};
}