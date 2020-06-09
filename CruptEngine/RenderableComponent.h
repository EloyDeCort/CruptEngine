#pragma once
#include "Texture2D.h"
#include "SDL.h"

namespace crupt
{
	struct RenderableComponent
	{
		Texture2D* m_Texture{};
		SDL_Rect m_SrcRect{0,0,0,0};
		int m_ScaleFactor{1};
	};
}