#pragma once

namespace crupt
{
	struct SpriteComponent
	{
		unsigned int m_FrameCount = 0;
		unsigned int m_CurrentFrame = 0;
		int m_AnimationRate = 10;
		float m_TotalTime = 0.f;
		int m_ScaleFactor = 1;

	};
}