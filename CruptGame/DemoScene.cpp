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


    tmx::Map map;

    if (map.load("../Data/TestMap.tmx"))
    {
        
        std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;
        if (map.isInfinite())
        {
            std::cout << "Map is infinite.\n";
        }

        const auto& mapProperties = map.getProperties();
        std::cout << "Map has " << mapProperties.size() << " properties" << std::endl;
        for (const auto& prop : mapProperties)
        {
            std::cout << "Found property: " << prop.getName() << std::endl;
            std::cout << "Type: " << int(prop.getType()) << std::endl;
        }

        std::cout << std::endl;

        const auto& layers = map.getLayers();
        std::cout << "Map has " << layers.size() << " layers" <<  std::endl;
        for (const auto& layer : layers)
        {
            std::cout << "Found Layer: " << layer->getName() << std::endl;
            std::cout << "Layer Type: " << int(layer->getType()) << std::endl;

            if (layer->getType() == tmx::Layer::Type::Group)
            {
                std::cout << "Checking sublayers" << std::endl;
                const auto& sublayers = layer->getLayerAs<tmx::LayerGroup>().getLayers();
                std::cout << "LayerGroup has " << sublayers.size() << " layers" << std::endl;
                for (const auto& sublayer : sublayers)
                {
                    std::cout << "Found Layer: " << sublayer->getName() << std::endl;
                    std::cout << "Layer Type: " << int(sublayer->getType()) << std::endl;
                }
            }

            if(layer->getType() == tmx::Layer::Type::Object)
            {
                const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
                std::cout << "Found " << objects.size() << " objects in layer" << std::endl;
                for(const auto& object : objects)
                {
                    std::cout << "Object " << object.getUID() << ", " << object.getName() << std::endl;
                    const auto& properties = object.getProperties();
                    std::cout << "Object has " << properties.size() << " properties" << std::endl;
                    for(const auto& prop : properties)
                    {
                        std::cout << "Found property: " << prop.getName() << std::endl;
                        std::cout << "Type: " << int(prop.getType()) << std::endl;
                    }

                    if (!object.getTilesetName().empty())
                    {
                        std::cout << "Object uses template tile set " << object.getTilesetName() << "\n";
                    }
                }
            }

            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                const auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
                if (tiles.empty())
                {
                    const auto& chunks = layer->getLayerAs<tmx::TileLayer>().getChunks();
                    if (chunks.empty())
                    {
                        std::cout << "Layer has missing tile data\n";
                    }
                    else
                    {
                        std::cout << "Layer has " << chunks.size() << " tile chunks.\n";
                    }
                }
                else
                {
                    std::cout << "Layer has " << tiles.size() << " tiles.\n";
                }
            }

            const auto& properties = layer->getProperties();
            std::cout << properties.size() << " Layer Properties:" << std::endl;
            for (const auto& prop : properties)
            {
                std::cout << "Found property: " << prop.getName() << std::endl;
                std::cout << "Type: " << int(prop.getType()) << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Failed loading map" << std::endl;
    }

    const std::vector<tmx::Tileset>& tilesets{map.getTilesets()};

    std::vector<Entity> mapEntityVector;

    for(int i{}; i < tilesets[0].getTiles().size(); ++i)
    {
        std::string testPath{tilesets[0].getTiles()[i].imagePath};
        testPath = testPath.substr(2);
        float x = float(tilesets[0].getTiles()[i].imagePosition.x) + 16* i;
        float y = float(tilesets[0].getTiles()[i].imagePosition.y) + 50;

        mapEntityVector.push_back(pCoordinator.CreateEntity());
	    pCoordinator.AddComponent<RenderableComponent>(mapEntityVector[i], RenderableComponent{ResourceManager::GetInstance().LoadTexture(testPath,renderer)});
	    pCoordinator.AddComponent<TransformComponent>(mapEntityVector[i], TransformComponent{glm::vec3(x,y,0.f)});
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
