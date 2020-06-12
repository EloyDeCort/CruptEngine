#include "CruptEnginePCH.h"
#include "MaitaMovementSystem.h"
#include "Components.h"
#include "GameComponents.h"
#include "SignalHandler.h"

crupt::MaitaMovementSystem::~MaitaMovementSystem()
{
}

void crupt::MaitaMovementSystem::Init()
{
}

void crupt::MaitaMovementSystem::PreUpdate(float)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;
	for (Entity entity : m_Entities)
	{
		MaitaComponent& maitaComp = coordinator->GetComponent<MaitaComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		RenderableComponent& renderComp = coordinator->GetComponent<RenderableComponent>(entity);
		TransformComponent& transComp = coordinator->GetComponent<TransformComponent>(entity);

		if(maitaComp.shouldDie)
		{
			toDelete.push_back(entity);
			continue;
		}

		if(boxComp.m_ColDirX == eDirection::LEFT || boxComp.m_ColDirX == eDirection::RIGHT)
		{
			maitaComp.flipped = !maitaComp.flipped;
			renderComp.m_Flip = !renderComp.m_Flip;
		}
		

		TransformComponent& playerTransComp = coordinator->GetComponent<TransformComponent>(maitaComp.player1);

		if(playerTransComp.position.y < transComp.position.y - maitaComp.jumpOffset)
		{
			float distance =  transComp.position.x - playerTransComp.position.x;
			if(fabs(distance) > maitaComp.maxDistanceOffset)
			{
				if(distance < 0.f)
				{
					maitaComp.flipped = false;
					renderComp.m_Flip = false;
				}
				else
				{
					maitaComp.flipped = true;
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
				if(!maitaComp.flipped)
				{
					movPhysicsComp.m_Force.x = maitaComp.movSpeed;
				}
				else
				{
					movPhysicsComp.m_Force.x = -maitaComp.movSpeed;
				}
			}
	}

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}
