#include "CruptEnginePCH.h"
#include "SceneManager.h"
#include "Scene.h"

void crupt::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

crupt::SceneManager::~SceneManager()
{
	for (Scene* scene : m_Scenes)
	{
		delete scene;
		scene = nullptr;
	}
}

void crupt::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

crupt::Scene& crupt::SceneManager::CreateScene(const std::string& name)
{
	crupt::Scene* scene = new Scene(name);
	m_Scenes.push_back(scene);
	return *scene;
}
