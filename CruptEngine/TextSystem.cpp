#include "CruptEnginePCH.h"
#include "TextSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Font.h"
#include "Texture2D.h"

using namespace crupt;

TextSystem::~TextSystem()
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	for (const Entity& entity : m_Entities)
	{
		TextComponent& text = coordinator.GetComponent<TextComponent>(entity);
		RenderableComponent& textureComp = coordinator.GetComponent<RenderableComponent>(entity);

		delete textureComp.pTexture;
		textureComp.pTexture = nullptr;

		delete text.pFont;
		text.pFont = nullptr;
	}
}

void TextSystem::Init(SDL_Renderer* renderer)
{
	m_Renderer = renderer;
}

void TextSystem::Update(float dt)
{
	dt;
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	for (const Entity& entity : m_Entities)
	{
		TextComponent& text = coordinator.GetComponent<TextComponent>(entity);
		if (text.needsUpdate)
		{
			const SDL_Color color = { Uint8(text.color.r), Uint8(text.color.g), Uint8(text.color.b)  }; // only white text is supported now
			const auto surf = TTF_RenderText_Blended(text.pFont->GetFont(), text.text.c_str(), color);
			if (surf == nullptr) 
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(m_Renderer, surf);
			if (texture == nullptr) 
			{
				throw std::runtime_error(std::string("Create text pTexture from surface failed: ") + SDL_GetError());
			}
			SDL_FreeSurface(surf);
			RenderableComponent& textureComp = coordinator.GetComponent<RenderableComponent>(entity);
			delete textureComp.pTexture;
			textureComp.pTexture = new Texture2D(texture);
			text.needsUpdate = false;
		}
	}
}

void TextSystem::SetText(Entity& entity, const std::string& text)
{
	std::set<crupt::Entity>::iterator find = m_Entities.find(entity);
	if(find == m_Entities.end())
		throw std::exception("TextSystem::SetText - [ERROR] System does not contain this entity");
	
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	TextComponent& textComp = coordinator.GetComponent<TextComponent>(entity);
	textComp.text = text;
	textComp.needsUpdate = true;
}