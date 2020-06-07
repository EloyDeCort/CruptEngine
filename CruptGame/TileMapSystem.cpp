#include "TileMapSystem.h"
#include <SDL.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/TileLayer.hpp>
#include "ResourceManager.h"

//We make use of TMXLite to parse simple .tmx files.
//TMX Files are made & provided with the program "TILED".
//This allows for quick level creation & loading.

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
	//Loading the map using tmxlite
    tmx::Map map;
	bool loaded = map.load("../Data/Level3.tmx");
   
	if(!loaded)
		return;

	//get the map dimensions
	const tmx::Vector2u mapDimensions{map.getTileCount()};
	unsigned int rows{mapDimensions.y};
	unsigned int cols{mapDimensions.x};

	//get the tile size
	tmx::Vector2u tileSize = map.getTileSize();
	unsigned int tileWidth = tileSize.x;
	unsigned int tileHeight = tileSize.y;

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

	//Load the map layers
	auto& mapLayers = map.getLayers();
	for(auto& layer : mapLayers)
	{
		//Check if the layer contains tiles
		if(layer->getType() != tmx::Layer::Type::Tile)
		{
			continue;
		}

		//If we have a tile layer, we can cast it to one.
		auto* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());

		//Get all the tiles from the layer.
		const std::vector<tmx::TileLayer::Tile>& layerTiles = tileLayer->getTiles();
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
                tempTile.yPos = size_t(y) * tileHeight;
				//Initialize the tile based on the position etc.
				m_Tiles.push_back(tempTile);
			}
		 }
	}

}

void crupt::TileMapSystem::Render()
{
	//Loop through all tiles and give them to the renderer
	for(auto& tile : m_Tiles)
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