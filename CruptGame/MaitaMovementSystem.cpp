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
		

			TransformComponent& playerTransComp = coordinator->GetComponent<TransformComponent>(maitaComp.player1);
			float distance =  transComp.position.x - playerTransComp.position.x;

			if(playerTransComp.position.y < transComp.position.y - maitaComp.jumpOffset)
			{
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

				if(abs(playerTransComp.position.y - transComp.position.y) < maitaComp.jumpOffset)
				{
					if(fabs(distance) <= maitaComp.maxBoulderDistanceOffset)
					{
						if(maitaComp.totalTime > maitaComp.chargeCoolDown)
						{

							BoulderComponent boulderComp;
							boulderComp.flipped = maitaComp.flipped;
							boulderComp.pos = transComp.position;
							SignalHandler<BoulderComponent>::GetInstance().Publish(boulderComp);


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
