#pragma once
#include "System.h"
#include "Texture2D.h"
#include <vector>
#include <map>

struct SDL_Renderer;
namespace tmx
{
	class Map;
}
namespace crupt
{
	struct Tile
	{
		unsigned int id;
		unsigned int xPos;
		unsigned int yPos;

		//Standard
		unsigned int width = 16;
		unsigned int height = 16;
	};

	class TileMapSystem : public System
	{
	public:
		TileMapSystem();
		virtual ~TileMapSystem();
		void Init(SDL_Renderer* renderer);
		void Render();

	private:
		void InitTileSet(const tmx::Map& map);
		bool AddLevel(const std::string& loc);

		using TileVector = std::vector<Tile>;
		using TileVectorMap = std::map<size_t, TileVector>;

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		std::vector<Texture2D*> m_pTileTextures;

		TileVectorMap m_TilesMap;
		std::vector<Tile> m_Tiles;

		int m_CurrentLevel;
		int m_TotalLevels;
		SDL_Renderer* m_pRenderer;
	};
}

