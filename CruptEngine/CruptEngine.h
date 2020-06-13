#pragma once
struct SDL_Window;
#include "ECSCoordinator.h"
#include "Systems.h"


//Systems classes
namespace crupt
{

	class CruptEngine
	{
	public:
		void Initialize();
		void Cleanup();
		void Run();

	protected:
		virtual void InitGame() = 0;
		RenderSystem* m_pRenderSystem;
	private:
		void RegisterComponents();
		void RegisterSystems();
		SDL_Window* m_Window{};
	};
}