#pragma once
#include <map>
#include "Texture2D.h" 
struct SDL_Rect;
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

	struct TileMapComponent
	{
		using TileVector = std::vector<Tile>;
		using CollisionVector = std::vector<SDL_Rect>;
		using TileVectorMap = std::map<size_t, TileVector>;
		using CollisionVectorMap = std::map<size_t, CollisionVector>;

		std::vector<Texture2D*> pTileTextures;

		TileVectorMap tilesMap;
		std::vector<Tile> tiles;

		CollisionVectorMap solidCollisionsMap;
		CollisionVectorMap platformCollisionsMap;

		int currentLevel = 0;
		int totalLevels = 0;
	};
}