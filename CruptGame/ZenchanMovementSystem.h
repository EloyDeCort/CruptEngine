#pragma once
#include "System.h"
#include "ECSCoordinator.h"

struct SDL_Renderer;
namespace crupt
{
	class ZenchanMovementSystem : public System
	{
	public:
		virtual ~ZenchanMovementSystem();
		void Init();
		void PreUpdate(float dt);

	private:
	};
}


