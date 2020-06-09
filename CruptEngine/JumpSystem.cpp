#include "CruptEnginePCH.h"
#include "JumpSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"

crupt::JumpSystem::~JumpSystem()
{
}

void crupt::JumpSystem::Init()
{
}

void crupt::JumpSystem::Update(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> entities{};
	for (Entity entity : m_Entities)
	{
		//Get the transform & Renderable component
		TransformComponent& transform = coordinator->GetComponent<TransformComponent>(entity);
		const JumpComponent& jumpComponent = coordinator->GetComponent<JumpComponent>(entity);

		//Jump (Temp)
		transform.position.y -= jumpComponent.jumpHeight * dt;
		std::cout << "[LOG]: JUMPING" << std::endl;
		entities.push_back(entity);
	}
	
	for (Entity entity : entities)
	{
		coordinator->RemoveComponent<JumpComponent>(entity);
	}
}
