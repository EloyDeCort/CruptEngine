#pragma once

namespace crupt
{
	enum class PlayerAnimState
	{
		IDLE,
		RUNNING
	};
	
	struct PlayerStateComponent
	{
		PlayerAnimState m_AnimationState;
	};
}