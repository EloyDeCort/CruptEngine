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
	//Update the sprite of the entity
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(entity);
	RenderableComponent& renderComp = coordinator->GetComponent<RenderableComponent>(entity);

	SDL_Texture* tex = renderComp.pTexture->GetSDLTexture();

	spriteComp.totalTime += dt;
	spriteComp.currentFrame = int(fmod((spriteComp.totalTime * spriteComp.animationRate), spriteComp.frameCount));

	int width{};
	int height{};
	SDL_Rect rect{};
	SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);

	rect.h = height;
	rect.w = width / spriteComp.frameCount;
	rect.y = 0;
	rect.x = rect.w * spriteComp.currentFrame;
	renderComp.srcRect = rect;
	renderComp.scaleFactor = spriteComp.scaleFactor;
}

void SpriteSystem::Update(float dt)
{
	//Sprites can be easily multithreaded
	std::vector<std::future<void>> futures;
	for (Entity entity : m_Entities)
	{
		futures.push_back(std::async(std::launch::async, EntityUpdate, entity, dt));
	}
}
