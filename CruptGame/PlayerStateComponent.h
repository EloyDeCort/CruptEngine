#pragma once
#include "SpriteComponent.h"
#include "Texture2D.h"
#include <vector>

namespace crupt
{
	enum class PlayerAnimState
	{
		IDLE,
		RUNNING
	};
	
	struct StateSprite
	{
		SpriteComponent m_SpriteData;
		Texture2D* m_Texture;
	};

	struct PlayerStateComponent
	{
		PlayerAnimState m_AnimationState;

		std::vector<StateSprite> m_pStateSprites;
	};
}