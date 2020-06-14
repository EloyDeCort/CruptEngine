#include "CruptEnginePCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Texture2D.h"
#include "Font.h"

crupt::ResourceManager::~ResourceManager()
{
	std::unordered_map<std::string, Texture2D*>::iterator it;
	for(it = m_pTextures.begin(); it != m_pTextures.end(); ++it)
	{
		delete it->second;
	}
	m_pTextures.clear();

	std::unordered_map<std::string, Font*>::iterator itFont;
	for(itFont = m_pFonts.begin(); itFont != m_pFonts.end(); ++itFont)
	{
		delete itFont->second;
	}
	m_pFonts.clear();
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
	std::unordered_map<std::string, Texture2D*>::iterator it = m_pTextures.find(file);
	if(it != m_pTextures.end())
	{
		return it->second;
	}
	
	const auto fullPath = m_DataPath + file;
	SDL_Texture* texture = IMG_LoadTexture(pRenderer, fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load pTexture: ") + SDL_GetError());
	}

	Texture2D* newTex = new Texture2D(texture);
	m_pTextures[file] = newTex;
	return newTex;

}

crupt::Font* crupt::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	std::unordered_map<std::string, Font*>::iterator it = m_pFonts.find(file + std::to_string(size));
	if(it != m_pFonts.end())
	{
		return it->second;
	}

	Font* newFont = new Font(m_DataPath + file, size);
	m_pFonts[file + std::to_string(size)] = newFont;
	return newFont;
}

