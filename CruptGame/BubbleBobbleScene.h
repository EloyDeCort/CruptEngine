#pragma once
#include "GameScene.h"
#include "tmxlite\Tileset.hpp"
#include <map>

namespace crupt
{
	struct tile
	{
		unsigned int id;
		unsigned int xPos;
		unsigned int yPos;
	};

	class BubbleBobbleScene : public GameScene
	{
	public:
		BubbleBobbleScene();
		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void Render() override;
	private:
		Entity m_FpsCounter;
		std::vector<Texture2D*> m_TileTextures;
		std::vector<tile> m_Tiles;
		//Systems
		TextSystem* m_pTextSystem;
		FPSSystem* m_pFPSSystem;
		RenderSystem* m_pRenderSystem;
		JumpSystem* m_pJumpSystem;
	};
}

