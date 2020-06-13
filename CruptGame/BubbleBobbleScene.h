#pragma once
#include "GameScene.h"
#include "Tileset.hpp"
#include <map>

namespace crupt
{
	//Predefines
	class JumpSystem;
	class PlayerStateSystem;
	class BubbleMovementSystem;
	class ZenchanMovementSystem;
	class SpawnEnemySystem;
	class HealthDisplaySystem;
	class ScoreDisplaySystem;
	class DropMovementSystem;
	class LevelStateSystem;

	enum class GameMode
	{
		SINGLE,
		COOP,
		VERSUS
	};

	class BubbleBobbleScene : public GameScene
	{
	public:
		BubbleBobbleScene();
		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void FixedUpdate(float dt) override;
		virtual void Render() override;
		virtual void SceneLoaded() override;
		virtual void SceneUnloaded() override;
	private:
		void InitSystems();
		void InitEntities();
		void InitPlayer1();
		void InitPlayer2();
		void InitEnemies();
		void InitTextures();

		Entity m_FpsCounter;

		//Systems (Used to have a specific update loop for each scene, only using the systems we need for that scene)
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
		ZenchanMovementSystem* m_pZenchanMovementSystem;
		SpawnEnemySystem* m_pSpawnEnemySystem;
		HealthDisplaySystem* m_pHealthDisplaySystem;
		DropMovementSystem* m_pDropMovementSystem;
		ScoreDisplaySystem* m_pScoreDisplaySystem;
		LevelStateSystem* m_pLevelStateSystem;

		GameMode m_GameMode;

		//Font
		crupt::Font* m_pFont;

		Entity m_Player1;
		Entity m_Player2;

		FMOD::Channel* m_pChannel;
		FMOD::Sound* m_pBGMusic;
	};
}

