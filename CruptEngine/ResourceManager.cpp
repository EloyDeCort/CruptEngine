#include "CruptEnginePCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Texture2D.h"
#include "Font.h"

crupt::ResourceManager::~ResourceManager()
{
	for(size_t i{}; i < m_pTextures.size(); ++i)
	{
		delete m_pTextures[i];
	}
	m_pTextures.clear();
}

void crupt::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

crupt::Texture2D* crupt::ResourceManager::LoadTexture(const std::string& file, SDL_Renderer* pRenderer)
{

	const auto fullPath = m_DataPath + file;
	SDL_Texture* texture = IMG_LoadTexture(pRenderer, fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	Texture2D* newTex = new Texture2D(texture);
	m_pTextures.push_back(newTex);

	return newTex;
}

crupt::Font* crupt::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return new Font(m_DataPath + file, size);
}

