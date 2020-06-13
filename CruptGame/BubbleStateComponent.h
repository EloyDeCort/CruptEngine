#pragma once
#include "SpriteComponent.h"
#include "Texture2D.h"
#include "PlayerStateComponent.h"
#include <vector>

namespace crupt
{
	enum class BubbleAnimState
	{
		NORMAL,
		ZENCHAN,
		MAITA
	};

	struct BubbleStateComponent
	{
		Entity target;

		BubbleAnimState m_AnimationState;
		std::vector<StateSprite> stateSprites;
	};
}