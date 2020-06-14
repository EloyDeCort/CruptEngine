//SOURCES:
//https://amanotes.com/using-swept-aabb-to-detect-and-process-collision/
//https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/

#include "CruptEnginePCH.h"
#include "CollisionSystem.h"
#include "Components.h"
#include "ECSCoordinator.h"
#include <SDL.h>
#include "Texture2D.h"
#include <thread>
#include <future>
using namespace crupt;

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;

	//Time delay to ensure no callbacks before this.
	m_TimeDelay = 0.1f;
	m_TotalTime = 0.f;
}

void crupt::CollisionSystem::InitMap(Entity mapEntity)
{
	m_MapEntity = mapEntity;
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	m_TileComp = &coordinator->GetComponent<TileMapComponent>(m_MapEntity);
}


void CollisionSystem::Update(float dt)
{
	
	m_TotalTime+=dt;
	if(m_TotalTime < m_TimeDelay)
		return;

	for (Entity entity : m_Entities)
	{
		ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		CollisionCallbackComponent& colCallbackComp = coordinator->GetComponent<CollisionCallbackComponent>(entity);
		Box playerBox{};
		playerBox.rect = boxComp.collisionRect;
		playerBox.velocity = movPhysicsComp.velocity;


		//Looking for the lowest collision time and match the final direction for that specific collision.
		//This is done using the swept aabb.
		//X and Y is split up to ensure collision in both directions
		float lowestColTimeX = 1.f;
		float lowestColTimeY = 1.f;
		eDirection finalDirX = eDirection::NONE;
		eDirection finalDirY = eDirection::NONE;
		
		//Checking collision with the solid sides of the maps
		for(const SDL_Rect& collision : m_TileComp->solidCollisionsMap.at(m_TileComp->currentLevel))
		{
			Box wallBox{};
			wallBox.rect = collision;
			wallBox.velocity = glm::vec2{0.f,0.f};

			eDirection result = eDirection::NONE;
			//Getting the SweptAABB time
			float collisionTime = SweptImproved(playerBox, wallBox, result);

			if(collisionTime < 1.f)
			{
				//Checking if the collision callback component has a valid collision
					if(colCallbackComp.onCollision != nullptr)
					{
						//Call specific on collision callback
						colCallbackComp.onCollision(entity, m_MapEntity, result);
					}

				if(result == eDirection::LEFT || result == eDirection::RIGHT)
				{
					if(collisionTime < lowestColTimeX)
					{
						lowestColTimeX = collisionTime;
						finalDirX = result;
					}
				}
				else if(result == eDirection::UP || result == eDirection::DOWN)
				{
					if(collisionTime < lowestColTimeY)
					{
						lowestColTimeY = collisionTime;
						finalDirY = result;
					}
				}
			}
		}
		

		//If we ignore collisions with platforms, we do not need to run this check.
		if(!boxComp.ignorePlatforms)
		{
			for(const SDL_Rect& collision : m_TileComp->platformCollisionsMap.at(m_TileComp->currentLevel))
			{
				Box wallBox{};
				wallBox.rect = collision;
				wallBox.velocity = glm::vec2{0.f,0.f};

				eDirection result = eDirection::NONE;
				float collisionTime = SweptImproved(playerBox, wallBox, result, true);

				if(collisionTime < 1.f)
				{
					if(result == eDirection::LEFT || result == eDirection::RIGHT)
					{
						if(collisionTime < lowestColTimeX)
						{
							lowestColTimeX = collisionTime;
							finalDirX = result;
						}
					}
					else if(result == eDirection::UP || result == eDirection::DOWN)
					{
						if(collisionTime < lowestColTimeY)
						{
							lowestColTimeY = collisionTime;
							finalDirY = result;
						}
					}
				}
			}
		}

		//Check if we want to collide with enemies
		if(!boxComp.ignoreEntities)
		{
			for (Entity entity2 : m_Entities)
			{
				if(entity == entity2)
					continue;

				MovePhysicsComponent& movPhysicsComp2 = coordinator->GetComponent<MovePhysicsComponent>(entity2);
				BoxCollisionComponent& boxComp2 = coordinator->GetComponent<BoxCollisionComponent>(entity2);

				Box entity2Box{};
				entity2Box.rect = boxComp2.collisionRect;
				entity2Box.velocity = movPhysicsComp2.velocity;

				eDirection result = eDirection::NONE;
				float collisionTime = SweptImproved(playerBox, entity2Box, result, false);

				//Simple AABB to do moving vs moving collision callbacks.
				if(IsColliding(playerBox, entity2Box))
				{
					//Call specific on collision callback
					colCallbackComp.onCollision(entity, entity2, result);
				}

				if(collisionTime < 1.f)
				{
					//Checking if the collision callback component has a valid collision
				
					if(result == eDirection::LEFT || result == eDirection::RIGHT)
					{
						if(collisionTime < lowestColTimeX)
						{
							lowestColTimeX = collisionTime;
							finalDirX = result;
						}
					}
					else if(result == eDirection::UP || result == eDirection::DOWN)
					{
						if(collisionTime < lowestColTimeY)
						{
							lowestColTimeY = collisionTime;
							finalDirY = result;
						}
					}
				}
			}
		}

		//Set the lowest entry times on the current collision component.
		boxComp.entryTimeX = lowestColTimeX;
		boxComp.entryTimeY = lowestColTimeY;
		boxComp.colDirX = finalDirX;	
		boxComp.colDirY = finalDirY;
	}
}

void crupt::CollisionSystem::Reset()
{
	m_TotalTime = 0.f;
}

bool crupt::CollisionSystem::IsColliding(const Box& obj, const Box& other)
{
	//AABB check
	float left = float(other.rect.x - (obj.rect.x + obj.rect.w));
	float top = float((other.rect.y + other.rect.h) - obj.rect.y);
	float right = float((other.rect.x + other.rect.w) - obj.rect.x);
	float bottom = float(other.rect.y - (obj.rect.y + obj.rect.h));

	return !(left > 0.f || right < 0.f || top < 0.f || bottom > 0.f);

}


float crupt::CollisionSystem::SweptImproved(const Box& obj, const Box& other, eDirection& colDirection, bool platform)
{
	//SWEPT AABB Algorithm to check collision and ensure no ghosting/
	float dxEntry{}, dxExit{};
	float dyEntry{}, dyExit{};

	Box broadBox = GetSweptBroadphase(obj);

	if(!IsColliding(broadBox, other))
	{
		return 1.f;
	}

	if(obj.velocity.x > 0.f)
	{
		dxEntry = float(other.rect.x - (obj.rect.x + obj.rect.w));
		dxExit = float((other.rect.x + other.rect.w) - obj.rect.x);
	}
	else
	{
		dxEntry = float((other.rect.x + other.rect.w) - obj.rect.x);
		dxExit =  float(other.rect.x - (obj.rect.x + obj.rect.w));
	}

	if(obj.velocity.y > 0.f)
	{
		dyEntry = float( other.rect.y - (obj.rect.y + obj.rect.h));
		dyExit = float((other.rect.y + other.rect.h) - obj.rect.y);
	}
	else
	{
		dyEntry = float((other.rect.y + other.rect.h) - obj.rect.y);
		dyExit =  float(other.rect.y - (obj.rect.y + obj.rect.h));
	}
	
	float txEntry{}, txExit{};
	float tyEntry{}, tyExit{};

	if(obj.velocity.x < FLT_EPSILON && obj.velocity.x > -FLT_EPSILON)
	{
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		txEntry = dxEntry / obj.velocity.x;
		txExit = dxExit / obj.velocity.x;
	}

	if(obj.velocity.y < FLT_EPSILON && obj.velocity.y > -FLT_EPSILON)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / obj.velocity.y;
		tyExit = dyExit / obj.velocity.y;
	}

	float entryTime = max(txEntry, tyEntry);
	float exitTime = min(txExit, tyExit);

	if(entryTime > exitTime || (txEntry < 0.f && tyEntry < 0.f) || txEntry > 1.f || tyEntry > 1.f)
	{
		return 1.f;
	}

	if(txEntry > tyEntry)
	{
		if(dxEntry > 0.f)
		{
			colDirection = eDirection::RIGHT;
		//	printf("Right\n");
		}
		else
		{
			colDirection = eDirection::LEFT;
			//printf("Left\n");
		}
	}
	else
	{
		if(dyEntry > 0.f)
		{
			colDirection = eDirection::UP;
			//printf("Up\n");
		}
		else
		{
			colDirection = eDirection::DOWN;
			if(obj.rect.y > other.rect.y)
			{
				colDirection = eDirection::UP;
				if(platform)
				{
					return 1.f;
				}
				//printf("Up\n");
			}
			else
			{
				//printf("Down\n");
			}
		}
	}

	return entryTime;

}



Box crupt::CollisionSystem::GetSweptBroadphase(const Box& object)
{
	//Make a box from one collision object to the other object.
	//This is used to optimize our checks.
	SDL_Rect result;
	result.x = object.velocity.x > 0.f ? int(object.rect.x) : int(object.rect.x + object.velocity.x);
	result.y = object.velocity.y > 0.f ? int(object.rect.y) : int(object.rect.y + object.velocity.y);
	result.w = int(object.rect.w + abs(object.velocity.x));
	result.h = int(object.rect.h + abs(object.velocity.y));

	Box res{};
	res.rect = result;

	return res;
}
