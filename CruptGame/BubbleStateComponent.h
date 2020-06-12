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
		MAITA
	};

	struct BubbleStateComponent
	{
		BubbleAnimState m_AnimationState;
		std::vector<StateSprite> m_pStateSprites;
	};
}