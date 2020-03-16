#include "CruptEnginePCH.h"
#include "TextSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "Renderer.h"
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
		TextureComponent& texture = coordinator.GetComponent<TextureComponent>(entity);
		TextComponent& text = coordinator.GetComponent<TextComponent>(entity);
		delete text.m_pFont;
		text.m_pFont = nullptr;
		delete texture.m_Texture;
		texture.m_Texture = nullptr;
	}
}

void TextSystem::Render()
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	for (const Entity& entity : m_Entities)
	{
		TextureComponent& texture = coordinator.GetComponent<TextureComponent>(entity);
		glm::vec3 pos = coordinator.GetComponent<TransformComponent>(entity).position;
		Renderer::GetInstance().RenderTexture(*texture.m_Texture, pos.x, pos.y);
	}
}


void TextSystem::Update()
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	for (const Entity& entity : m_Entities)
	{
		TextComponent& text = coordinator.GetComponent<TextComponent>(entity);
		if (text.m_NeedsUpdate)
		{
			const SDL_Color color = { Uint8(text.m_Color.r), Uint8(text.m_Color.g), Uint8(text.m_Color.b)  }; // only white text is supported now
			const auto surf = TTF_RenderText_Blended(text.m_pFont->GetFont(), text.m_Text.c_str(), color);
			if (surf == nullptr) 
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr) 
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_FreeSurface(surf);
			TextureComponent& textureComp = coordinator.GetComponent<TextureComponent>(entity);
			delete textureComp.m_Texture;
			textureComp.m_Texture = new Texture2D(texture);
			text.m_NeedsUpdate = false;
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
	textComp.m_Text = text;
	textComp.m_NeedsUpdate = true;
}