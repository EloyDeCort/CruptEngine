#pragma once
#include "GameScene.h"
#include "Tileset.hpp"
#include <map>

namespace crupt
{
	class TileMapSystem;
	class JumpSystem;
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
		PhysicsSystem* m_pPhysicsSystem;

		FMOD::Channel* m_pChannel;
		FMOD::Sound* m_pBGMusic;
	};
}

