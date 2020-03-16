#include "CruptEnginePCH.h"
//#include "Scene.h"
//#include "GameObject.h"
//
//using namespace crupt;
//
//unsigned int Scene::m_IdCounter = 0;
//
//Scene::Scene(const std::string& name) : m_Name(name) {}
//
//Scene::~Scene()
//{
//	for (GameObject* gameObj : m_pObjects)
//	{
//		delete gameObj;
//		gameObj = nullptr;
//	}
//}
//
//void Scene::Add(GameObject* object)
//{
//	m_pObjects.push_back(object);
//}
//
//void Scene::Update()
//{
//	for(GameObject* object : m_pObjects)
//	{
//		object->Update();
//	}
//}
//
//void Scene::Render() const
//{
//	for (const auto& object : m_pObjects)
//	{
//		object->Render();
//	}
//}
//
