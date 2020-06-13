#pragma once
#include "System.h"
#include "Texture2D.h"
#include <vector>
#include <map>
#include "GameComponents.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

struct SDL_Renderer;
struct SDL_Rect;

namespace crupt
{

	class HealthDisplaySystem : public System
	{
	public:
		HealthDisplaySystem();
		virtual ~HealthDisplaySystem();
		void Init(SDL_Renderer* renderer);
		void SetPlayer1(Entity player);
		void SetPlayer2(Entity player);
		void Render();
	
	private:
		void RenderTexture(const Texture2D& texture, float x, float y) const;

		Texture2D* m_pHealthP2Texture;
		Texture2D* m_pHealthP1Texture;

		SDL_Renderer* m_pRenderer;

		Entity m_Player1;
		Entity m_Player2;
		int m_NrOfPlayers;
		float m_HealthOffset;

		glm::vec2 m_Player1Offset;
		glm::vec2 m_Player2Offset;
	};
}

