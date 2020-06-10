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
	struct TileMapComponent;

	class TileMapSystem : public System
	{
	public:
		TileMapSystem();
		virtual ~TileMapSystem();
		void Init(SDL_Renderer* renderer);
		void InitMap(Entity mapEntity);
		void Render();

	private:
		void InitTileSet(const tmx::Map& map);
		bool AddLevel(const std::string& loc);
		void RenderTexture(const Texture2D& texture, float x, float y) const;

	
		SDL_Renderer* m_pRenderer;

		Entity m_MapEntity;
		TileMapComponent* m_TileComp;

		unsigned int m_UIOffset;
	};
}

