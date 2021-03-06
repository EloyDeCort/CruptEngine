#pragma once
#include "GameScene.h"

namespace crupt
{
	class BBMainMenuScene : public GameScene
	{
	public:
		BBMainMenuScene();
		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void FixedUpdate(float dt) override;
		virtual void Render() override;
		virtual void SceneLoaded() override;
		virtual void SceneUnloaded() override;
	private:
		void InitAudio();

		//Systems
		RenderSystem* m_pRenderSystem;
		float m_TotalTime;

		FMOD::Channel* m_pChannel;
		FMOD::Sound* m_pBGMusic;
		FMOD::ChannelGroup* m_pChannelGroup;
	};
}

