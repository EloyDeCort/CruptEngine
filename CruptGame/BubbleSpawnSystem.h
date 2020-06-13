#pragma once
#include "System.h"
#include "GameComponents.h"
#include "Components.h"

struct SDL_Renderer;
struct Texture2D;
namespace crupt
{
	class BubbleSpawnSystem : public System
	{
	public:
		virtual ~BubbleSpawnSystem();
		void Init(SDL_Renderer* renderer);
		void OnDispatch(const BubbleComponent& component);

	private:
		SDL_Renderer* m_pRenderer;
		BubbleStateComponent m_BubbleStateCompP1;
		BubbleStateComponent m_BubbleStateCompP2;
		BoxCollisionComponent m_BoxCollisionComp;
	};
}

