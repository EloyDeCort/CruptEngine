#pragma once
#include "SpriteComponent.h"
#include "Texture2D.h"
#include <vector>

namespace crupt
{
	enum class PlayerAnimState
	{
		IDLE,
		RUNNING,
		SPITBUBBLE
	};
	
	struct StateSprite
	{
		SpriteComponent spriteData;
		Texture2D* pTexture;
	};

	struct PlayerStateComponent
	{
		PlayerAnimState animationState;

		std::vector<StateSprite> pStateSprites;
	};
}