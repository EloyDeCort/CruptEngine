#pragma once
#include "System.h"
#include "GameComponents.h"

struct SDL_Renderer;
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
	};
}

