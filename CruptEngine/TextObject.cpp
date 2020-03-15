#include "CruptEnginePCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextObject::TextObject(const std::string& text, Font* font) 
	: m_NeedsUpdate(true), m_Text(text), m_pFont(font), m_pTexture(nullptr)
{ }

void dae::TextObject::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
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
		m_pTexture = new Texture2D(texture);
		m_NeedsUpdate = false;
	}
}

dae::TextObject::~TextObject()
{
	delete m_pTexture;
	m_pTexture = nullptr;
	delete m_pFont;
	m_pFont = nullptr;
}

void dae::TextObject::Render() const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextObject::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextObject::SetPosition(const float x, const float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}


