#pragma once
#include "System.h"
#include "ECSCoordinator.h"

struct SDL_Renderer;
namespace crupt
{
	class MaitaMovementSystem : public System
	{
	public:
		virtual ~MaitaMovementSystem();
		void Init();
		void PreUpdate(float dt);

	private:
	};
}


