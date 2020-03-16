#include "CruptEnginePCH.h"
#include "GraphicsSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "Renderer.h"
#include "Texture2D.h"

using namespace crupt;


GraphicsSystem::~GraphicsSystem()
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	for (const Entity& entity : m_Entities)
	{
		TextureComponent& texture = coordinator.GetComponent<TextureComponent>(entity);
		delete texture.m_Texture;
		texture.m_Texture = nullptr;
	}
}

void GraphicsSystem::Render()
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	for (const Entity& entity : m_Entities)
	{
		TextureComponent& texture = coordinator.GetComponent<TextureComponent>(entity);
		glm::vec3 pos = coordinator.GetComponent<TransformComponent>(entity).position;
		Renderer::GetInstance().RenderTexture(*texture.m_Texture, pos.x, pos.y);
	}
}