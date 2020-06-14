#pragma once
#include "GameScene.h"

namespace FMOD
{
	class Channel;
	class ChannelGroup;
	class Sound;
}

namespace crupt
{
	class BBLoseScene : public GameScene
	{
	public:
		BBLoseScene();
		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void FixedUpdate(float dt) override;
		virtual void Render() override;
		virtual void SceneLoaded() override;
		virtual void SceneUnloaded() override;
		virtual void SetScoreP1(int score);
		virtual void SetScoreP2(int score);

	private:
		void InitAudio();

		//Systems
		RenderSystem* m_pRenderSystem;

		TextSystem* m_pTextSystem;

		float m_TotalTime;

		Font* m_pFont;
		int m_ScoreP1;
		int m_ScoreP2;

		FMOD::Channel* m_pChannel;
		FMOD::Sound* m_pBGMusic;
		FMOD::ChannelGroup* m_pChannelGroup;
	};
}

