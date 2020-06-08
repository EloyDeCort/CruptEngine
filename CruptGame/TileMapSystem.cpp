#include "TileMapSystem.h"
#include <SDL.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/TileLayer.hpp>
#include "ResourceManager.h"
#include <iostream>

//We make use of TMXLite to parse simple .tmx files.
//TMX Files are made & provided with the program "TILED".
//This allows for quick level creation & loading.

crupt::TileMapSystem::TileMapSystem()
	: m_CurrentLevel{0}
	, m_TotalLevels{0}
	, m_UIOffset{50}
{
}

crupt::TileMapSystem::~TileMapSystem()
{
	for (size_t i{}; i < m_pTileTextures.size(); ++i)
	{
		delete m_pTileTextures[i];
	}

	m_pTileTextures.clear();

}

void crupt::TileMapSystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
	//Load tileset from the first map. 
	//Since we are using the same tileset for all maps, we can simply load it once.
	tmx::Map map;
	bool loaded = map.load("../Data/Level1.tmx");
   
	if(!loaded)
		return;

	InitTileSet(map);

	//Add our levels.
	AddLevel("../Data/Level1.tmx");
	AddLevel("../Data/Level2.tmx");
	AddLevel("../Data/Level3.tmx");

}

bool crupt::TileMapSystem::AddLevel(const std::string& loc)
{
	//Loading the map using tmxlite
    tmx::Map map;
	bool loaded = map.load(loc);
   
	if(!loaded)
		return false;

	//get the map dimensions
	const tmx::Vector2u mapDimensions{map.getTileCount()};
	unsigned int rows{mapDimensions.y};
	unsigned int cols{mapDimensions.x};

	//get the tile size
	tmx::Vector2u tileSize = map.getTileSize();
	unsigned int tileWidth = tileSize.x;
	unsigned int tileHeight = tileSize.y;

	//Load the map layers
	const std::vector<tmx::Layer::Ptr>& mapLayers = map.getLayers();
	for(const tmx::Layer::Ptr& layer : mapLayers)
	{
		//Check if the layer contains tiles
		if(layer->getType() != tmx::Layer::Type::Tile)
		{
			if(layer->getType() == tmx::Layer::Type::Object)
			{
				std::cout << "OBJECT\n";
			}
			continue;
		}

		//If we have a tile layer, we can cast it to one.
		const tmx::TileLayer& tileLayer = layer->getLayerAs<tmx::TileLayer>();

		//Get all the tiles from the layer.
		const std::vector<tmx::TileLayer::Tile>& layerTiles = tileLayer.getTiles();
		 for (unsigned int y = 0; y < rows; ++y) 
		 {
            for (unsigned int x = 0; x < cols; ++x) 
			{
				//Calculate the tile index
				unsigned int tileIndex = x + (y * cols);
				unsigned int curGid = layerTiles[tileIndex].ID;

				//check if it's an empty grid
				if (curGid == 0) 
				{
					continue;
				}

				Tile tempTile;
				tempTile.id = curGid;
				tempTile.xPos = size_t(x) * tileWidth;
                tempTile.yPos = m_UIOffset + size_t(y) * tileHeight;
				//Initialize the tile based on the position etc.
				m_TilesMap[m_TotalLevels].push_back(tempTile);
			}
		 }
	}
	
	m_TotalLevels++;
	return true;
}

void crupt::TileMapSystem::InitTileSet(const tmx::Map& map)
{
	//Load the tile set
	std::vector<tmx::Tileset> tileSets = map.getTilesets();
	for(tmx::Tileset& tset : tileSets)
	{
		//insert all tiles into the vector of tile textures.
		for(const tmx::Tileset::Tile& tile : tset.getTiles())
		{
			std::string path = tile.imagePath;
			path = path.substr(2);
			m_pTileTextures.push_back(ResourceManager::GetInstance().LoadTexture(path,m_pRenderer));
		}
	}
}

void crupt::TileMapSystem::Render()
{
	//Loop through all tiles and give them to the renderer
	for(auto& tile : m_TilesMap.at(m_CurrentLevel))
	{
		//tile.id - 1 will give us what texture we defined in Tiled.
		RenderTexture(*m_pTileTextures[tile.id - 1], float(tile.xPos), float(tile.yPos));
	}
}


void crupt::TileMapSystem::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(m_pRenderer, texture.GetSDLTexture(), nullptr, &dst);
}