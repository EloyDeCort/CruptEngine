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
		static bool IsColliding(const Box& obj, const Box& other);
		static float SweptImproved(const Box& obj, const Box& other, eDirection& colDirection, bool platform = false);
		static void EntityUpdate(Entity entity, std::set<crupt::Entity>* entities, TileMapComponent* tileComp);

	private:
		static Box GetSweptBroadphase(const Box& object);
		Entity m_MapEntity;
		TileMapComponent* m_TileComp;
		SDL_Renderer* m_pRenderer;
	};
}

