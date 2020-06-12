#include "CruptEnginePCH.h"
#include "SpriteSystem.h"
#include "Components.h"
#include <SDL.h>
#include "Texture2D.h"
#include <thread>
#include <future>
using namespace crupt;

SpriteSystem::~SpriteSystem()
{
}

void SpriteSystem::Init()
{
}

static void EntityUpdate(Entity entity, float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(entity);
	RenderableComponent& renderComp = coordinator->GetComponent<RenderableComponent>(entity);

	SDL_Texture* tex = renderComp.m_Texture->GetSDLTexture();

	spriteComp.m_TotalTime += dt;
	spriteComp.m_CurrentFrame = int(fmod((spriteComp.m_TotalTime * spriteComp.m_AnimationRate), spriteComp.m_FrameCount));

	int width{};
	int height{};
	SDL_Rect rect{};
	SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);

	rect.h = height;
	rect.w = width / spriteComp.m_FrameCount;
	rect.y = 0;
	rect.x = rect.w * spriteComp.m_CurrentFrame;
	renderComp.m_SrcRect = rect;
	renderComp.m_ScaleFactor = spriteComp.m_ScaleFactor;
}

void SpriteSystem::Update(float dt)
{
	std::vector<std::future<void>> futures;
	for (Entity entity : m_Entities)
	{
		futures.push_back(std::async(std::launch::async, EntityUpdate, entity, dt));
	}
}
