#include "CruptEnginePCH.h"
#include "DemoScene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/TileLayer.hpp>


#include <iostream>

crupt::DemoScene::DemoScene()
	: GameScene(L"DemoScene")
	, m_FpsCounter{}
{

}

void crupt::DemoScene::Init()
{
	ECSCoordinator& pCoordinator = crupt::ECSCoordinator::GetInstance();
	Entity background = pCoordinator.CreateEntity();

	//INITIALIZE THE SYSTEMS
	m_pRenderSystem = pCoordinator.GetSystem<RenderSystem>();
	m_pTextSystem = pCoordinator.GetSystem<TextSystem>();
	m_pFPSSystem = pCoordinator.GetSystem<FPSSystem>();
	m_pJumpSystem = pCoordinator.GetSystem<JumpSystem>();


	SDL_Renderer* renderer{m_pRenderSystem->GetSDLRenderer()};
	pCoordinator.AddComponent<RenderableComponent>(background, RenderableComponent{ResourceManager::GetInstance().LoadTexture("background.jpg",renderer)});
	pCoordinator.AddComponent<TransformComponent>(background, TransformComponent{glm::vec3(0.f,0.f,0.f)});


	crupt::Font* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	Entity text = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(text, RenderableComponent{});
	pCoordinator.AddComponent<TransformComponent>(text, TransformComponent{glm::vec3(80.f,20.f,0.f)});
	pCoordinator.AddComponent<TextComponent>(text, TextComponent{bool{true}, std::string("Programming 4 Assignment"), font, glm::vec3(255.f, 0.f, 0.f)});

	crupt::Font* fontFps = ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	m_FpsCounter = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(m_FpsCounter, RenderableComponent{});
	pCoordinator.AddComponent<TransformComponent>(m_FpsCounter, TransformComponent{glm::vec3(0.f,0.f,0.f)});
	pCoordinator.AddComponent<TextComponent>(m_FpsCounter, TextComponent{bool{true}, std::string("FPS: "), fontFps, glm::vec3(255.f, 255.f, 0.f)});
	pCoordinator.AddComponent<FPSComponent>(m_FpsCounter, FPSComponent{});

	Entity player = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<TransformComponent>(player, TransformComponent{glm::vec3(216.f,180.f,0.f)});

    Entity logo = pCoordinator.CreateEntity();
	pCoordinator.AddComponent<RenderableComponent>(logo, RenderableComponent{ResourceManager::GetInstance().LoadTexture("logo.png",renderer)});
	pCoordinator.AddComponent<TransformComponent>(logo, TransformComponent{glm::vec3(216.f,180.f,0.f)});
    InputManager::GetInstance().SetPlayer(logo);


	std::vector<Entity> entities{};
    tmx::Map map;
	bool loaded = map.load("../Data/Level2.tmx");
   
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
		//crupt::Texture2D* tex = ResourceManager::GetInstance().LoadTexture(tset.getImagePath(),renderer);
		for(auto& tile : tset.getTiles())
		{
			std::string path = tile.imagePath;
			path = path.substr(2);
			m_TileTextures.push_back(ResourceManager::GetInstance().LoadTexture(path,renderer));
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

				tile tempTile;
				tempTile.id = curGid;
				tempTile.xPos = size_t(x) * tileWidth;
                tempTile.yPos = size_t(y) * tileHeight;
				m_Tiles.push_back(tempTile);
			}
		 }

		for(auto& tile : m_Tiles)
		{

			Entity temp = pCoordinator.CreateEntity();
			entities.push_back(temp);
			pCoordinator.AddComponent<RenderableComponent>(temp, RenderableComponent{m_TileTextures[tile.id - 1]});
			pCoordinator.AddComponent<TransformComponent>(temp, TransformComponent{glm::vec3(tile.xPos,tile.yPos,0.f)});
		}
	}


}

void crupt::DemoScene::Update(float dt)
{	
	m_pFPSSystem->SetText(m_FpsCounter, "FPS: " + std::to_string(m_pFPSSystem->GetFPS(m_FpsCounter)));
	m_pTextSystem->Update(dt);
	m_pFPSSystem->Update(m_FpsCounter, dt);
	m_pRenderSystem->Update(dt);
	m_pJumpSystem->Update(dt);

}

void crupt::DemoScene::Render()
{
	
}
