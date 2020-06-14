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

bool crupt::GameScene::OwnsEntity(Entity entity) const
{
	//Return true if the entity is found Unused)
	std::set<crupt::Entity>::iterator find = m_Entities.find(entity);
	return find != m_Entities.end();
}

void crupt::GameScene::AddEntity(Entity entity)
{
	//These gamescene commands are not used anymore
	m_Entities.insert(entity);
}
