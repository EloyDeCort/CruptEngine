#include "CruptEnginePCH.h"
#include "ZenchanMovementSystem.h"
#include "Components.h"
#include "GameComponents.h"
#include "SignalHandler.h"

crupt::ZenchanMovementSystem::~ZenchanMovementSystem()
{
}

void crupt::ZenchanMovementSystem::Init()
{
}

void crupt::ZenchanMovementSystem::PreUpdate(float)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;
	for (Entity entity : m_Entities)
	{
		ZenchanComponent& zenchanComp = coordinator->GetComponent<ZenchanComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		RenderableComponent& renderComp = coordinator->GetComponent<RenderableComponent>(entity);
		TransformComponent& transComp = coordinator->GetComponent<TransformComponent>(entity);

		if(zenchanComp.shouldDie)
		{
			toDelete.push_back(entity);
			continue;
		}

		if(boxComp.m_ColDirX == eDirection::LEFT || boxComp.m_ColDirX == eDirection::RIGHT)
		{
			zenchanComp.flipped = !zenchanComp.flipped;
			renderComp.m_Flip = !renderComp.m_Flip;
		}
		

		TransformComponent& playerTransComp = coordinator->GetComponent<TransformComponent>(zenchanComp.player1);

		if(playerTransComp.position.y < transComp.position.y - zenchanComp.jumpOffset)
		{
			float distance =  transComp.position.x - playerTransComp.position.x;
			if(fabs(distance) > zenchanComp.maxDistanceOffset)
			{
				if(distance < 0.f)
				{
					zenchanComp.flipped = false;
					renderComp.m_Flip = false;
				}
				else
				{
					zenchanComp.flipped = true;
					renderComp.m_Flip = true;
				}
			}
			else
			{
				JumpComponent jumpComp;
				jumpComp.m_Target = entity;
				SignalHandler<JumpComponent>::GetInstance().Publish(jumpComp);
			}
		}
		
			if(boxComp.m_ColDirY == eDirection::DOWN)
			{
				if(!zenchanComp.flipped)
				{
					movPhysicsComp.m_Force.x = zenchanComp.movSpeed;
				}
				else
				{
					movPhysicsComp.m_Force.x = -zenchanComp.movSpeed;
				}
			}
	}

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}
