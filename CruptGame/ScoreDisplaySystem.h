#pragma once
#include "System.h"
#include "Texture2D.h"
#include <vector>
#include "GameComponents.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

struct SDL_Renderer;
struct SDL_Rect;
namespace crupt
{

	class ScoreDisplaySystem : public System
	{
	public:
		ScoreDisplaySystem();
		virtual ~ScoreDisplaySystem();
		void Init(SDL_Renderer* renderer);
		void SetPlayer1(Entity player, Entity scoreEntity);
		void SetPlayer2(Entity player, Entity scoreEntity);
		void Render();
		void Reset();
		void OnDispatch(const ScoreComponent& component);
	
	private:

		SDL_Renderer* m_pRenderer;

		Entity m_Player1;
		Entity m_Player2;

		Entity m_ScorePlayer1;
		Entity m_ScorePlayer2;
		int m_NrOfPlayers;

		glm::vec2 m_Player1Offset;
		glm::vec2 m_Player2Offset;
	};
}

