#pragma once
#include "System.h"
#include "ECSCoordinator.h"
#include "Texture2D.h"

struct SDL_Renderer;
struct SDL_Rect;

namespace crupt
{
	class SpriteSystem : public System
	{
	public:
		virtual ~SpriteSystem();
		void Init(SDL_Renderer* renderer);
		void Update(float dt);

	private:
		void RenderTexture(const Texture2D& texture, float x, float y, SDL_Rect* srcRect) const;
		//Member variables
		SDL_Renderer* m_Renderer{};
	};
}

