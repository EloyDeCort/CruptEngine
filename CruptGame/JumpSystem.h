#pragma once
#include "System.h"
#include "GameComponents.h"

namespace FMOD
{
	class Channel;
	class Sound;
}

namespace crupt
{
	class JumpSystem : public System
	{
	public:
		JumpSystem();
		virtual ~JumpSystem();
		void Init();
		void OnDispatch(const JumpComponent& component);

	private:
		FMOD::Channel* m_pChannel;
		FMOD::Sound* m_pSoundEffect;
	};
}

