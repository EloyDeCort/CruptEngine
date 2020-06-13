#pragma once
#include "System.h"
#include "GameComponents.h"
#include "Components.h"

struct SDL_Renderer;
namespace crupt
{
	class CollisionCallbackSystem;
	class SpawnDropSystem : public System
	{
	public:
		virtual ~SpawnDropSystem();
		void Init(SDL_Renderer* renderer, CollisionCallbackSystem* colCallbackSystem);
		void OnDispatch(DropComponent component);

	private:
		void SpawnMelon(const glm::vec2& pos);
		SDL_Renderer* m_pRenderer;
		CollisionCallbackSystem* m_pCollisionCallbackSystem;

		int m_MelonScore;
		int m_FriesScore;
	};
}

