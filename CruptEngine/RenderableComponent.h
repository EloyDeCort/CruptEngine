#pragma once
#include "Texture2D.h"
#include "SDL.h"

namespace crupt
{
	struct RenderableComponent
	{
		Texture2D* m_Texture{};
		SDL_Rect srcRect{0,0,0,0};
	};
}