#include "CruptEnginePCH.h"
#include "RenderSystem.h"
#include "Components.h"
#include <SDL.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
using namespace crupt;

RenderSystem::~RenderSystem()
{

}

void RenderSystem::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	ImGui::CreateContext();
	ImGuiSDL::Initialize(m_Renderer, 640, 480);
	m_Coordinator = &ECSCoordinator::GetInstance();
}

void RenderSystem::Update(float dt)
{	
	//SDL_RenderClear(m_Renderer);
	dt;
	for (const Entity& entity : m_Entities)
	{
		//Get the transform & Renderable component
		const TransformComponent& transform = m_Coordinator->GetComponent<TransformComponent>(entity);
		const RenderableComponent& rendererable = m_Coordinator->GetComponent<RenderableComponent>(entity);

		glm::vec3 pos = transform.position;

		//Render the texture from the renderable component
		RenderTexture(*rendererable.m_Texture, pos.x, pos.y);
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

void RenderSystem::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(m_Renderer, texture.GetSDLTexture(), nullptr, &dst);
}

void RenderSystem::Destroy()
{

	//Clean up all textures
	for (const Entity& entity : m_Entities)
	{
		RenderableComponent& rendererable = m_Coordinator->GetComponent<RenderableComponent>(entity);
		delete rendererable.m_Texture;
		rendererable.m_Texture = nullptr;
	}

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void RenderSystem::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(m_Renderer, texture.GetSDLTexture(), nullptr, &dst);
}
