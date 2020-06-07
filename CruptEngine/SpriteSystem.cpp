#include "CruptEnginePCH.h"
#include "SpriteSystem.h"
#include "Components.h"
#include <SDL.h>
#include "Texture2D.h"
using namespace crupt;

SpriteSystem::~SpriteSystem()
{

}

void SpriteSystem::Init(SDL_Renderer* renderer)
{
	m_Renderer = renderer;
}

void SpriteSystem::Update(float dt)
{
	dt;
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	for (Entity entity : m_Entities)
	{
		SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(entity);
		RenderableComponent& renderComp = coordinator->GetComponent<RenderableComponent>(entity);

		SDL_Texture* tex = renderComp.m_Texture->GetSDLTexture();

		spriteComp.m_TotalTime += dt;
		spriteComp.m_CurrentFrame = int(spriteComp.m_TotalTime * spriteComp.m_AnimationRate) % spriteComp.m_FrameCount;

		int width{};
		int height{};
		SDL_Rect rect{};
		SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);

		rect.h = height;
		rect.w = width / spriteComp.m_FrameCount;
		rect.y = 0;
		rect.x = rect.w * spriteComp.m_CurrentFrame;
		renderComp.srcRect = rect;
	}

	SDL_SetRenderTarget(m_Renderer, nullptr);
}

void SpriteSystem::RenderTexture(const Texture2D& texture, const float x, const float y, SDL_Rect* srcRect) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x) + 40;
	dst.y = static_cast<int>(y) + 40;
	dst.w = srcRect->w;
	dst.h = srcRect->h;

	SDL_RenderCopy(m_Renderer, texture.GetSDLTexture(), srcRect, &dst);
}