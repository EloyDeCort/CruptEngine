#pragma once
struct SDL_Window;
#include "ECSCoordinator.h"
#include "Systems.h"


//Systems classes
namespace crupt
{
	struct Settings
	{
		static const int m_WindowWidth = 640;
		static const int m_WindowHeight = 530;
	};

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
		//static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};

	};
}