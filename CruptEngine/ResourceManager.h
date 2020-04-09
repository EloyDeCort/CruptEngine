#pragma once
#include "Singleton.h"

	struct SDL_Renderer;
namespace crupt
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file, SDL_Renderer* pRenderer) const;
		Font* LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}
