#include "CruptEnginePCH.h"
#include "RenderSystem.h"
#include "Components.h"
#include <SDL.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "CruptEngine.h"
using namespace crupt;

RenderSystem::~RenderSystem()
{

	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
}

void RenderSystem::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	ImGui::CreateContext();
	ImGuiSDL::Initialize(m_Renderer, Settings::windowWidth, Settings::windowHeight);
	m_Coordinator = &ECSCoordinator::GetInstance();
}

void RenderSystem::Render()
{	
	//SDL_RenderClear(m_Renderer);
	for (const Entity& entity : m_Entities)
	{	
		//Get the transform & Renderable component
		const TransformComponent& transform = m_Coordinator->GetComponent<TransformComponent>(entity);
		const RenderableComponent& renderable = m_Coordinator->GetComponent<RenderableComponent>(entity);

		glm::vec2 pos = transform.position;

		//Render the texture from the renderable component
		RenderTexture(*renderable.pTexture, pos.x, pos.y, &renderable.srcRect, renderable.scaleFactor, renderable.flip);
	}
	
	ImGuiDebug();

	//SDL_RenderPresent(m_Renderer);
}

void RenderSystem::ImGuiDebug()
{
	bool open = true;
	open;
	ImGui::NewFrame();
	//ImGui::SetNextWindowSize(ImVec2{500.f,50.f});
	//ImGui::SetNextWindowPos(ImVec2{0.f,0.f});
	//ImGui::Begin("Test", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::End();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}

void RenderSystem::RenderTexture(const Texture2D& texture, const float x, const float y, const SDL_Rect* srcRect, int scale, bool flip) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	if(srcRect->w == 0 && srcRect->h == 0)
	{
		SDL_RenderCopy(m_Renderer, texture.GetSDLTexture(), nullptr, &dst);
	}
	else
	{
		dst.w = srcRect->w * scale;
		dst.h = srcRect->h * scale;

		if(flip)
		{
			SDL_RenderCopyEx(m_Renderer, texture.GetSDLTexture(), srcRect, &dst, 0.f, nullptr, SDL_FLIP_HORIZONTAL);
		}
		else
		{
			SDL_RenderCopy(m_Renderer, texture.GetSDLTexture(), srcRect, &dst);
		}
	}
}

void RenderSystem::Destroy()
{

	//Clean up all textures
	for (const Entity& entity : m_Entities)
	{
		RenderableComponent& rendererable = m_Coordinator->GetComponent<RenderableComponent>(entity);
		delete rendererable.pTexture;
		rendererable.pTexture = nullptr;
	}

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void RenderSystem::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const SDL_Rect* srcRect, int scale, bool flip) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	if(srcRect->w == 0 && srcRect->h == 0)
	{
		SDL_RenderCopy(m_Renderer, texture.GetSDLTexture(), nullptr, &dst);
	}
	else
	{
		dst.w = srcRect->w * scale;
		dst.h = srcRect->h * scale;
		if(flip)
		{
			SDL_RenderCopyEx(m_Renderer, texture.GetSDLTexture(), srcRect, &dst, 0.f, nullptr, SDL_FLIP_HORIZONTAL);
		}
		else
		{
			SDL_RenderCopy(m_Renderer, texture.GetSDLTexture(), srcRect, &dst);
		}
	}
}
