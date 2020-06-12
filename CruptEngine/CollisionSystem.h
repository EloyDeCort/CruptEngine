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
		 bool IsColliding(const Box& obj, const Box& other);
		float SweptImproved(const Box& obj, const Box& other, eDirection& colDirection, bool platform = false);
		void EntityUpdate(crupt::Entity entity);

	private:
		Box GetSweptBroadphase(const Box& object);
		Entity m_MapEntity;
		TileMapComponent* m_TileComp;
		SDL_Renderer* m_pRenderer;
	};
}

