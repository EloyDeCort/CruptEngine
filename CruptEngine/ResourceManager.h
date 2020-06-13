#pragma once
#include "Singleton.h"
#include <unordered_map>

	struct SDL_Renderer;
namespace crupt
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager() = default;
		virtual ~ResourceManager();
		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file, SDL_Renderer* pRenderer);
		Font* LoadFont(const std::string& file, unsigned int size);
	private:
		std::string m_DataPath;
		std::unordered_map<std::string,Texture2D*> m_pTextures;
		std::unordered_map<std::string,Font*> m_pFonts;
	};
}
