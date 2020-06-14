#pragma once
#include "SpriteComponent.h"
#include "Texture2D.h"
#include <vector>

namespace crupt
{
	//Player state 
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

	//Playerstate component to handle the player state easily
	struct PlayerStateComponent
	{
		PlayerAnimState animationState;

		std::vector<StateSprite> pStateSprites;
	};
}