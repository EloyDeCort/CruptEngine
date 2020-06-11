#include "CruptEnginePCH.h"
#include "SceneManager.h"
#include "GameScene.h"
#include <algorithm>

crupt::SceneManager::~SceneManager()
{
	for(GameScene* pScene : m_pScenes)
	{
		delete pScene;
	}
	m_pScenes.clear();
}

void crupt::SceneManager::AddScene(GameScene* pScene)
{
	if(std::find(m_pScenes.cbegin(), m_pScenes.cend(), pScene) != m_pScenes.cend())
	{
		//Already in scene vector
		throw std::exception("SceneManager::AddScene - [ERROR] trying to add scene that already exists.");
		//Return for safety
		return;
	}

	//Scene not yet added -> add it
	m_pScenes.push_back(pScene);

	if(m_ActiveScene == nullptr)
	{
		m_ActiveScene = pScene;
	}
}



void crupt::SceneManager::RemoveScene(GameScene* pScene)
{
	const std::vector<GameScene*>::iterator it{std::find(m_pScenes.begin(), m_pScenes.end(), pScene)};

	if(it != m_pScenes.end())
	{
		//Scene in vector -> Remove
		m_pScenes.erase(it);
	}
	else
	{
		//Not in scene vector.
		throw std::exception("SceneManager::RemoveScene - [ERROR] trying to remove scene that doesn't excist.");
		//Return for safety
		return;
	}
}

void crupt::SceneManager::SetActiveScene(const std::wstring& sceneName)
{
	const std::vector<GameScene*>::iterator it{find_if(m_pScenes.begin(), m_pScenes.end(), [sceneName](GameScene* scene)
		{
			return wcscmp(scene->GetSceneName().c_str(), sceneName.c_str()) == 0;
		})};

	if(it != m_pScenes.end())
	{
		//Found the scene, set the scene to the active one.
		m_NewActiveScene = *it;
	}
	else
	{
		//Scene Not found
		throw std::exception("SceneManager::SetActiveScene - [ERROR] trying to set active scene to a scene that does not exist.");
		//Return for safety
		return;
	}
}

crupt::GameScene* crupt::SceneManager::GetActiveScene()
{
	return m_ActiveScene;
}

void crupt::SceneManager::Init()
{
	if(m_ActiveScene)
	{
		//Make sure the scene is initialized.
		m_ActiveScene->Init();
	}
}

void crupt::SceneManager::Update(float dt)
{
	if (m_NewActiveScene != nullptr)
	{
		//Set New Scene
		m_ActiveScene = m_NewActiveScene;
		m_NewActiveScene = nullptr;
		m_ActiveScene->Init();
	}

	if(m_ActiveScene)
	{
		m_ActiveScene->Update(dt);
	}
}

void crupt::SceneManager::PostUpdate(float dt)
{
	if(m_ActiveScene)
	{
		m_ActiveScene->PostUpdate(dt);
	}
}

void crupt::SceneManager::Render()
{
	if(m_ActiveScene)
	{
		m_ActiveScene->Render();
	}
}
