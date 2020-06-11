#pragma once
#include "System.h"
#include "ECSCoordinator.h"
#include "Components.h"

struct SDL_Rect;
namespace crupt
{
	struct TileMapComponent;

	struct Box
	{
		SDL_Rect rect;

		glm::vec2 velocity;
	};

	class CollisionSystem : public System
	{
	public:
		virtual ~CollisionSystem();
		void Init(SDL_Renderer* renderer);
		void InitMap(Entity mapEntity);
		void Update(float dt);
		bool AABB(SDL_Rect b1, SDL_Rect b2);
		float SweptAABB(Box b1, Box b2, float& xNormal, float& yNormal);
		
	private:

		Entity m_MapEntity;
		TileMapComponent* m_TileComp;
		SDL_Renderer* m_pRenderer;
	};
}

