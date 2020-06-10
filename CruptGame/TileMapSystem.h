#pragma once
#include "System.h"
#include "Texture2D.h"
#include <vector>
#include <map>

struct SDL_Renderer;
struct SDL_Rect;
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
		using CollisionVector = std::vector<SDL_Rect>;
		using TileVectorMap = std::map<size_t, TileVector>;
		using CollisionVectorMap = std::map<size_t, CollisionVector>;

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		std::vector<Texture2D*> m_pTileTextures;

		TileVectorMap m_TilesMap;
		std::vector<Tile> m_Tiles;

		CollisionVectorMap m_SolidCollisionsMap;
		int m_CurrentLevel;
		int m_TotalLevels;
		SDL_Renderer* m_pRenderer;

		unsigned int m_UIOffset;
	};
}

