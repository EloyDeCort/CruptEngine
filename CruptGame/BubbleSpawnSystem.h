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
		void OnDispatch(BubbleComponent component);

	private:
		SDL_Renderer* m_pRenderer;
		Texture2D* m_pBubbleTexture;	
		RenderableComponent test;
	};
}

