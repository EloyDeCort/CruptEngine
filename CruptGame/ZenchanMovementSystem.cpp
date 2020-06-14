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


		if(boxComp.colDirX == eDirection::LEFT || boxComp.colDirX == eDirection::RIGHT)
		{
			zenchanComp.flipped = !zenchanComp.flipped;
			renderComp.flip = !renderComp.flip;
		}
		
		if(!zenchanComp.coOp)
		{
			TransformComponent& playerTransComp = coordinator->GetComponent<TransformComponent>(zenchanComp.player1);

			if(playerTransComp.position.y < transComp.position.y - zenchanComp.jumpOffset)
			{
				float distance =  transComp.position.x - playerTransComp.position.x;
				if(fabs(distance) > zenchanComp.maxDistanceOffset)
				{
					if(distance < 0.f)
					{
						zenchanComp.flipped = false;
						renderComp.flip = false;
					}
					else
					{
						zenchanComp.flipped = true;
						renderComp.flip = true;
					}
				}
				else
				{
					JumpComponent jumpComp;
					jumpComp.target = entity;
					SignalHandler<JumpComponent>::GetInstance().Publish(jumpComp);
				}
			}
		}
		else
		{
			//CO OP BEHAVIOR
			TransformComponent& playerTransComp1 = coordinator->GetComponent<TransformComponent>(zenchanComp.player1);
			HealthComponent& healthComp1 = coordinator->GetComponent<HealthComponent>(zenchanComp.player1);

			TransformComponent& playerTransComp2 = coordinator->GetComponent<TransformComponent>(zenchanComp.player2);
			HealthComponent& healthComp2 = coordinator->GetComponent<HealthComponent>(zenchanComp.player2);

			float distanceP1 =  transComp.position.x - playerTransComp1.position.x;
			float distanceP2 =  transComp.position.x - playerTransComp2.position.x;

			float finalDistanceX = 0.f;
			float finalPosY = 0.f;
			if(fabs(distanceP1) < fabs(distanceP2))
			{
				finalDistanceX = distanceP1;
				finalPosY = playerTransComp1.position.y;
			}
			else
			{
				finalDistanceX = distanceP2;
				finalPosY = playerTransComp2.position.y;
			}

			if(healthComp1.dead)
			{
				finalDistanceX = distanceP2;
				finalPosY = playerTransComp2.position.y;
			}

			if(healthComp2.dead)
			{
				finalDistanceX = distanceP1;
				finalPosY = playerTransComp1.position.y;
			}

			if(finalPosY < transComp.position.y - zenchanComp.jumpOffset)
			{
				if(fabs(finalDistanceX) > zenchanComp.maxDistanceOffset)
				{
					if(finalDistanceX < 0.f)
					{
						zenchanComp.flipped = false;
						renderComp.flip = false;
					}
					else
					{
						zenchanComp.flipped = true;
						renderComp.flip = true;
					}
				}
				else
				{
					JumpComponent jumpComp;
					jumpComp.target = entity;
					SignalHandler<JumpComponent>::GetInstance().Publish(jumpComp);
				}
			}
		}




			if(boxComp.colDirY == eDirection::DOWN)
			{
				if(!zenchanComp.flipped)
				{
					movPhysicsComp.force.x = zenchanComp.movSpeed;
				}
				else
				{
					movPhysicsComp.force.x = -zenchanComp.movSpeed;
				}
			}
	}

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}
