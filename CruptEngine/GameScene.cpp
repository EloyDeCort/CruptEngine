#include "CruptEnginePCH.h"
#include "GameScene.h"

crupt::GameScene::GameScene(const std::wstring& sceneName)
	: m_SceneName{sceneName}
{

}

crupt::GameScene::~GameScene()
{

}

const std::wstring& crupt::GameScene::GetSceneName() const
{
	return m_SceneName;
}
