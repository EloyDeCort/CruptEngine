#pragma once
#include "System.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "GameComponents.h"

struct SDL_Renderer;
namespace crupt
{
	class BubbleMovementSystem : public System
	{
	public:
		virtual ~BubbleMovementSystem();
		void Init(SDL_Renderer* renderer);
		void Update(float dt);
		void PreUpdate(float dt);

	private:
		void SpawnEnemy(BubbleAnimState state, const glm::vec2& pos);
		SDL_Renderer* m_pRenderer;
	};
}

