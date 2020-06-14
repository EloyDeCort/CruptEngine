#pragma once
#include "GameScene.h"
#include "Tileset.hpp"
#include <map>

namespace crupt
{
	class TileMapSystem;
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
		//Systems
		GameScene* m_GameScene;
		RenderSystem* m_pRenderSystem;
		float m_TotalTime;
	};
}

