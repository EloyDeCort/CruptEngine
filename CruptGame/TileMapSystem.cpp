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
}

void crupt::TileMapSystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
    tmx::Map map;
	bool loaded = map.load("../Data/Level3.tmx");
   
	if(!loaded)
		return;

	const tmx::Vector2u mapDimensions{map.getTileCount()};
	unsigned int rows{mapDimensions.y};
	unsigned int cols{mapDimensions.x};

	tmx::Vector2u tileSize = map.getTileSize();
	unsigned int tileWidth = tileSize.x;
	unsigned int tileHeight = tileSize.y;

	std::vector<tmx::Tileset> tileSets = map.getTilesets();
	for(tmx::Tileset& tset : tileSets)
	{
		for(auto& tile : tset.getTiles())
		{
			std::string path = tile.imagePath;
			path = path.substr(2);
			m_pTileTextures.push_back(ResourceManager::GetInstance().LoadTexture(path,m_pRenderer));
		}
	}


	auto& mapLayers = map.getLayers();
	for(auto& layer : mapLayers)
	{
		if(layer->getType() != tmx::Layer::Type::Tile)
		{
			continue;
		}
		auto* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());

		auto& layerTiles = tileLayer->getTiles();
		layerTiles;
		 for (unsigned int y = 0; y < rows; ++y) 
		 {
            for (unsigned int x = 0; x < cols; ++x) 
			{
				auto tileIndex = x + (y * cols);
				auto curGid = layerTiles[tileIndex].ID;
				if (curGid == 0) 
				{
                continue;
				}

				Tile tempTile;
				tempTile.id = curGid;
				tempTile.xPos = size_t(x) * tileWidth;
                tempTile.yPos = size_t(y) * tileHeight;
				m_Tiles.push_back(tempTile);
			}
		 }
	}

}

void crupt::TileMapSystem::Update(float dt)
{
	dt;
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