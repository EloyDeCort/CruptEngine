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
		void Init();
		void Update(float dt);

	private:
	};
}

