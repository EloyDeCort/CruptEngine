#pragma once
#include "Texture2D.h"
#include "SDL.h"

namespace crupt
{
	struct RenderableComponent
	{
		Texture2D* pTexture{nullptr};
		SDL_Rect srcRect{0,0,0,0};
		int scaleFactor{1};
		bool flip = false;
	};
}