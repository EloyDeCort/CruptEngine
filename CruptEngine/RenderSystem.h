#pragma once
#include "System.h"
#include "ECSCoordinator.h"
#include "Texture2D.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace crupt
{
	class RenderSystem : public System
	{
	public:
		virtual ~RenderSystem();
		void Init(SDL_Window* window);
		void Render();

		void Destroy();

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	private:
		//Helper Functions
		void RenderTexture(const Texture2D& texture, float x, float y, const SDL_Rect* srcRect, int scale, bool flip) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, const SDL_Rect* srcRect, int scale, bool flip) const;

		void ImGuiDebug();

		//Member variables
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_pWindow{};
		ECSCoordinator* m_Coordinator;
	};
}

