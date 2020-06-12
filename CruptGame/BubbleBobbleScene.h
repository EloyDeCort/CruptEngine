#pragma once
#include "GameScene.h"
#include "Tileset.hpp"
#include <map>

namespace crupt
{
	class JumpSystem;
	class PlayerStateSystem;
	class BubbleMovementSystem;
	class MaitaMovementSystem;
	class BubbleBobbleScene : public GameScene
	{
	public:
		BubbleBobbleScene();
		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void FixedUpdate(float dt) override;
		virtual void Render() override;
	private:
		void InitSystems();
		void InitEntities();
		void InitPlayers();
		void InitEnemies();

		Entity m_FpsCounter;
		//Systems
		TextSystem* m_pTextSystem;
		FPSSystem* m_pFPSSystem;
		RenderSystem* m_pRenderSystem;
		SpriteSystem* m_pSpriteSystem;
		JumpSystem* m_pJumpSystem;
		TileMapSystem* m_pTileMapSystem;
		PhysicsSystem* m_pPhysicsSystem;
		CollisionSystem* m_pCollisionSystem;
		PlayerStateSystem* m_pPlayerStateSystem;
		BubbleMovementSystem* m_pBubbleMovementSystem;
		MaitaMovementSystem* m_pMaitaMovementSystem;
		Entity m_Player1;
		Entity m_Player2;

		FMOD::Channel* m_pChannel;
		FMOD::Sound* m_pBGMusic;
	};
}

