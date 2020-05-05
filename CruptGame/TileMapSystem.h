#pragma once
#include "System.h"
#include "Texture2D.h"
#include <vector>

struct SDL_Renderer;
namespace crupt
{
	struct Tile
	{
		unsigned int id;
		unsigned int xPos;
		unsigned int yPos;
	};

	class TileMapSystem : public System
	{
	public:
		virtual ~TileMapSystem();
		void Init(SDL_Renderer* renderer);
		void Update(float dt);

	private:
		void RenderTexture(const Texture2D& texture, float x, float y) const;
		std::vector<Texture2D*> m_pTileTextures;
		std::vector<Tile> m_Tiles;
		SDL_Renderer* m_pRenderer;
	};
}

