#pragma once
#include "GameScene.h"


namespace crupt
{
	class JumpSystem;
	class DemoScene : public GameScene
	{
	public:
		DemoScene();
		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void FixedUpdate(float dt) override;
		virtual void Render() override;
		virtual void SceneLoaded() override;
		virtual void SceneUnloaded() override;
	private:
		Entity m_FpsCounter;
		//Systems
		TextSystem* m_pTextSystem;
		FPSSystem* m_pFPSSystem;
		RenderSystem* m_pRenderSystem;
		JumpSystem* m_pJumpSystem;
	};
}

