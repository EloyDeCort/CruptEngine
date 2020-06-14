#pragma once
#include "System.h"
#include "GameComponents.h"
#include "Components.h"

struct SDL_Renderer;
namespace crupt
{
	class CollisionCallbackSystem;
	class SpawnEnemySystem : public System
	{
	public:
		virtual ~SpawnEnemySystem();
		void Init(SDL_Renderer* renderer, CollisionCallbackSystem* colCallbackSystem);
		void OnDispatch(const SpawnEnemyComponent& component);
		void SetPlayer1(Entity player);
		void SetPlayer2(Entity player);
		void Reset();

	private:
		void SpawnZenchan(const glm::vec2& pos);
		SDL_Renderer* m_pRenderer;
		CollisionCallbackSystem* m_pCollisionCallbackSystem;
		Entity m_Player1;
		Entity m_Player2;
		int m_NrOfPlayers;
	};
}

