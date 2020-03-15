#include "CruptEnginePCH.h"
#include "Texture2D.h"
#include <SDL.h>

crupt::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* crupt::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

crupt::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
