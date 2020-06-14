#pragma once
#include "System.h"
struct SDL_Renderer;
namespace crupt
{
	class TextSystem : public System
	{
	public:
		virtual ~TextSystem();
		void Init(SDL_Renderer* renderer);
		void Update(float dt);
		void SetText(Entity& entity, const std::string& text);
		void Reset();
	private:
		SDL_Renderer* m_Renderer;
	};
}

