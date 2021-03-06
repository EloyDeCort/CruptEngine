#include "CruptEnginePCH.h"
#include "FPSSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
using namespace crupt;


FPSSystem::~FPSSystem()
{

}

int FPSSystem::GetFPS(Entity& entity) const
{
	std::set<crupt::Entity>::iterator find = m_Entities.find(entity);
	if(find == m_Entities.end())
		throw std::exception("TextSystem::SetText - [ERROR] System does not contain this entity");
	
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	FPSComponent& fpsComp = coordinator.GetComponent<FPSComponent>(entity);
	return fpsComp.fps;
}

void FPSSystem::Update(Entity entity, float deltaTime)
{
	//Increase FPS and display with text component
	m_Time += deltaTime;
	m_FpsCounter++;

	std::set<crupt::Entity>::iterator find = m_Entities.find(entity);
	if(find == m_Entities.end())
		throw std::exception("TextSystem::SetText - [ERROR] System does not contain this entity");
	
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();
	FPSComponent& fpsComp = coordinator.GetComponent<FPSComponent>(entity);
	if(m_Time > 1.f)
	{
		m_Time = 0.f;
		fpsComp.fps = m_FpsCounter;
		m_FpsCounter = 0;
	}
	
}