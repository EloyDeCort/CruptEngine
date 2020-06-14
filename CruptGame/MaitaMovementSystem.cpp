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

void crupt::MaitaMovementSystem::PreUpdate(float dt)
{
	//Update the movement of the maita and deleting all maitas marked for delete.
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;
	for (Entity entity : m_Entities)
	{
		MaitaComponent& maitaComp = coordinator->GetComponent<MaitaComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		RenderableComponent& renderComp = coordinator->GetComponent<RenderableComponent>(entity);
		SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(entity);
		TransformComponent& transComp = coordinator->GetComponent<TransformComponent>(entity);

		if(maitaComp.isPlayer)
		{
			maitaComp.totalTime += dt;

			HealthComponent& healthComp = coordinator->GetComponent<HealthComponent>(entity);
			if(healthComp.gotHit)
			{
				healthComp.totalTime += dt;
				if(healthComp.totalTime >= healthComp.hitDelay)
				{
					healthComp.gotHit = false;
					healthComp.totalTime = 0.f;
				}
			}

			 if(maitaComp.state == MaitaAnimState::CHARGING)
			 {
				if(maitaComp.totalTime > maitaComp.chargeTime)
				{
					//SPAWN A ROCK IN THE DIRECTION
					maitaComp.state = MaitaAnimState::WALKING;

					//Change Sprite
					StateSprite& newSprite = maitaComp.pStateSprites[int(maitaComp.state)];
					spriteComp.animationRate = newSprite.spriteData.animationRate;
					spriteComp.frameCount = newSprite.spriteData.frameCount;
					renderComp.pTexture = maitaComp.pStateSprites[int(maitaComp.state)].pTexture;
				}
			}

			 return;
		}

		if(maitaComp.shouldDie)
		{
			toDelete.push_back(entity);
			continue;
		}

		maitaComp.totalTime += dt;
		
		if(maitaComp.state == MaitaAnimState::WALKING)
		{

			if(boxComp.colDirX == eDirection::LEFT || boxComp.colDirX == eDirection::RIGHT)
			{
				maitaComp.flipped = !maitaComp.flipped;
				renderComp.flip = !renderComp.flip;
			}
		
			float finalDistanceX = 0.f;
			float finalPosY = 0.f;

			if(!maitaComp.coOp)
			{
				TransformComponent& playerTransComp = coordinator->GetComponent<TransformComponent>(maitaComp.player1);
				finalPosY = playerTransComp.position.y;

				if(playerTransComp.position.y < transComp.position.y - maitaComp.jumpOffset)
				{
					float distance =  transComp.position.x - playerTransComp.position.x;

					finalDistanceX = distance;

					if(fabs(distance) > maitaComp.maxDistanceOffset)
					{
						if(distance < 0.f)
						{
							maitaComp.flipped = false;
							renderComp.flip = false;
						}
						else
						{
							maitaComp.flipped = true;
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
				TransformComponent& playerTransComp1 = coordinator->GetComponent<TransformComponent>(maitaComp.player1);
				HealthComponent& healthComp1 = coordinator->GetComponent<HealthComponent>(maitaComp.player1);

				TransformComponent& playerTransComp2 = coordinator->GetComponent<TransformComponent>(maitaComp.player2);
				HealthComponent& healthComp2 = coordinator->GetComponent<HealthComponent>(maitaComp.player2);

				float distanceP1 =  transComp.position.x - playerTransComp1.position.x;
				float distanceP2 =  transComp.position.x - playerTransComp2.position.x;

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

				if(finalPosY < transComp.position.y - maitaComp.jumpOffset)
				{
					if(fabs(finalDistanceX) > maitaComp.maxDistanceOffset)
					{
						if(finalDistanceX < 0.f)
						{
							maitaComp.flipped = false;
							renderComp.flip = false;
						}
						else
						{
							maitaComp.flipped = true;
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
				if(!maitaComp.flipped)
				{
					movPhysicsComp.force.x = maitaComp.movSpeed;
				}
				else
				{
					movPhysicsComp.force.x = -maitaComp.movSpeed;
				}

 				if(fabs(finalPosY - transComp.position.y) < maitaComp.jumpOffset)
				{
					if(fabs(finalDistanceX) <= maitaComp.maxBoulderDistanceOffset)
					{
						if(maitaComp.totalTime > maitaComp.chargeCoolDown)
						{
							maitaComp.state = MaitaAnimState::CHARGING;

							//Change Sprite
							StateSprite& newSprite = maitaComp.pStateSprites[int(maitaComp.state)];
							spriteComp.animationRate = newSprite.spriteData.animationRate;
							spriteComp.frameCount = newSprite.spriteData.frameCount;
							renderComp.pTexture = maitaComp.pStateSprites[int(maitaComp.state)].pTexture;

							maitaComp.totalTime = 0.f;
						}
					}
				}
			}
		}
		else if(maitaComp.state == MaitaAnimState::CHARGING)
		{
			if(maitaComp.totalTime > maitaComp.chargeTime)
			{
				//SPAWN A ROCK IN THE DIRECTION
				BoulderComponent boulderComp;
				boulderComp.flipped = maitaComp.flipped;
				boulderComp.pos = transComp.position;
				SignalHandler<BoulderComponent>::GetInstance().Publish(boulderComp);
			
				maitaComp.totalTime = 0.f;

				maitaComp.state = MaitaAnimState::WALKING;

				//Change Sprite
				StateSprite& newSprite = maitaComp.pStateSprites[int(maitaComp.state)];
				spriteComp.animationRate = newSprite.spriteData.animationRate;
				spriteComp.frameCount = newSprite.spriteData.frameCount;
				renderComp.pTexture = maitaComp.pStateSprites[int(maitaComp.state)].pTexture;
			}
		}

		
	}

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}
