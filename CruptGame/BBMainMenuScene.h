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
		virtual void Render() override;
	private:

		//Systems
		RenderSystem* m_pRenderSystem;

		float m_TotalTime;
	};
}

