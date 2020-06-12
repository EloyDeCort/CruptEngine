#pragma once
#include "System.h"
#include "ECSCoordinator.h"

struct SDL_Renderer;
namespace crupt
{
	class BubbleMovementSystem : public System
	{
	public:
		virtual ~BubbleMovementSystem();
		void Init();
		void Update(float dt);
		void PreUpdate(float dt);

	private:
	};
}

