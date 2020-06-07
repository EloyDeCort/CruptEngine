#pragma once
#include "GameScene.h"
#include "tmxlite\Tileset.hpp"
#include <map>

namespace crupt
{
	class TileMapSystem;
	class BubbleBobbleScene : public GameScene
	{
	public:
		BubbleBobbleScene();
		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void Render() override;
	private:
		void InitSystems();
		void InitEntities();

		Entity m_FpsCounter;
		//Systems
		TextSystem* m_pTextSystem;
		FPSSystem* m_pFPSSystem;
		RenderSystem* m_pRenderSystem;
		SpriteSystem* m_pSpriteSystem;
		JumpSystem* m_pJumpSystem;
		TileMapSystem* m_pTileMapSystem;
	};
}
