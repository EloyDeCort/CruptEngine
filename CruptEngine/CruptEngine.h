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
		void LoadGame();
		void Cleanup();
		void Run();
	private:
		void RegisterComponents();
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};


		//Systems
		GraphicsSystem* m_pGraphicsSystem;
		TextSystem* m_pTextSystem;
		FPSSystem* m_pFPSSystem;
		Entity m_FpsCounter;
	};
}