#include "CruptEnginePCH.h"
#include "DropMovementSystem.h"
#include "ECSCoordinator.h"
#include "GameComponents.h"
#include "Components.h"

crupt::DropMovementSystem::~DropMovementSystem()
{
}

void crupt::DropMovementSystem::Init()
{
}

void crupt::DropMovementSystem::PreUpdate(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;

	for (Entity entity : m_Entities)
	{
		DropComponent& dropComp = coordinator->GetComponent<DropComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);

		if(dropComp.destroy)
		{
			toDelete.push_back(entity);
			continue;
		}


		dropComp.totalTime += dt;
		if(dropComp.totalTime < dropComp.movTime)
		{
			movPhysicsComp.force.x = 2.f;
			movPhysicsComp.force.y = -400.f;
		}

	}

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}
