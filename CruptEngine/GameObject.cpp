#include "CruptEnginePCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

crupt::GameObject::~GameObject()
{
	/*delete m_pTexture;
	m_pTexture = nullptr;*/
};

void crupt::GameObject::Update(){}

void crupt::GameObject::Render() const
{
	//const auto pos = m_Transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void crupt::GameObject::SetTexture(const std::string& filename)
{
	filename;
	/*m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);*/
}

void crupt::GameObject::SetPosition(float x, float y)
{
	x;
	y;
	//m_Transform.SetPosition(x, y, 0.0f);
}
