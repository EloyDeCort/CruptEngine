#include "CruptEnginePCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

dae::SceneManager::~SceneManager()
{
	for (Scene* scene : m_Scenes)
	{
		delete scene;
		scene = nullptr;
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	dae::Scene* scene = new Scene(name);
	m_Scenes.push_back(scene);
	return *scene;
}
