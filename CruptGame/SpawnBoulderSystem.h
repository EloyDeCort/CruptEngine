#pragma once
#include "System.h"
#include "GameComponents.h"
#include "Components.h"

struct SDL_Renderer;
namespace crupt
{
	class CollisionCallbackSystem;
	class SpawnBoulderSystem : public System
	{
	public:
		virtual ~SpawnBoulderSystem();
		void Init(SDL_Renderer* renderer, CollisionCallbackSystem* colCallbackSystem);
		void PreUpdate(float dt);
		void OnDispatch(const BoulderComponent& component);

	private:
		SDL_Renderer* m_pRenderer;
		CollisionCallbackSystem* m_pCollisionCallbackSystem;
	};
}

