//SOURCES:
//https://amanotes.com/using-swept-aabb-to-detect-and-process-collision/
//https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/

#include "CruptEnginePCH.h"
#include "CollisionSystem.h"
#include "Components.h"
#include "ECSCoordinator.h"
#include <SDL.h>
#include "Texture2D.h"
using namespace crupt;

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
}

void crupt::CollisionSystem::InitMap(Entity mapEntity)
{
	m_MapEntity = mapEntity;
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	m_TileComp = &coordinator->GetComponent<TileMapComponent>(m_MapEntity);
}

void CollisionSystem::Update(float dt)
{
	dt;
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	for (Entity entity : m_Entities)
	{
		VelocityComponent& velocityComp = coordinator->GetComponent<VelocityComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		Box playerBox{};
		playerBox.rect = boxComp.m_CollisionRect;
		playerBox.velocity = velocityComp.m_Velocity;

		float lowestColTime = 1.f;
		eDirection finalDir = eDirection::NONE;
		
		for(const SDL_Rect& collision : m_TileComp->m_SolidCollisionsMap.at(m_TileComp->m_CurrentLevel))
		{
			Box wallBox{};
			wallBox.rect = collision;
			wallBox.velocity = glm::vec2{0.f,0.f};

			eDirection result = eDirection::NONE;
			float collisionTime = SweptImproved(playerBox, wallBox, result);

			if(collisionTime < 1.f)
			{
				if(collisionTime < lowestColTime)
				{
					lowestColTime = collisionTime;
					finalDir = result;
				}
			}
			else
			{
				
			}

			SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(m_pRenderer, &boxComp.m_CollisionRect);

			SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		
		}
		
		//std::cout << int(finalDir) << std::endl;
		
		for(const SDL_Rect& collision : m_TileComp->m_PlatformCollisionsMap.at(m_TileComp->m_CurrentLevel))
		{
			Box wallBox{};
			wallBox.rect = collision;
			wallBox.velocity = glm::vec2{0.f,0.f};

			eDirection result = eDirection::NONE;
			float collisionTime = SweptImproved(playerBox, wallBox, result);

			if(collisionTime < 1.f)
			{
				if(collisionTime < lowestColTime)
				{
					lowestColTime = collisionTime;
					finalDir = result;
				}
			}
			else
			{
				
			}
				
		}

		boxComp.m_EntryTime = lowestColTime;
		boxComp.m_CollisionDir = finalDir;
	}
}

bool crupt::CollisionSystem::IsColliding(const Box& obj, const Box& other)
{
	float left = float(other.rect.x - (obj.rect.x + obj.rect.w));
	float top = float((other.rect.y + other.rect.h) - obj.rect.y);
	float right = float((other.rect.x + other.rect.w) - obj.rect.x);
	float bottom = float(other.rect.y - (obj.rect.y + obj.rect.h));

	return !(left > 0.f || right < 0.f || top < 0.f || bottom > 0.f);

}

float crupt::CollisionSystem::SweptAABB(const Box& b1, const Box& b2, float& xNormal, float& yNormal)
{
	float xInvEntry{}, yInvEntry{}; 
	float xInvExit{}, yInvExit{}; 

	//find distance between near and far side
	if(b1.velocity.x > 0.f)
	{
		xInvEntry = float(b2.rect.x - (b1.rect.x + b1.rect.w));
		xInvExit = float((b2.rect.x + b2.rect.w) - b1.rect.x);
	}
	else
	{
		xInvEntry = float((b2.rect.x + b2.rect.w) - b1.rect.x);
		xInvExit =  float(b2.rect.x - (b1.rect.x + b1.rect.w));
	}

	if(b1.velocity.y > 0.f)
	{
		yInvEntry = float(b2.rect.y - (b1.rect.y + b1.rect.h));
		yInvExit = float((b2.rect.y + b2.rect.h) - b1.rect.y);
	}
	else
	{
		yInvEntry = float((b2.rect.y + b2.rect.h) - b1.rect.y);
		yInvExit =  float(b2.rect.y - (b1.rect.y + b1.rect.h));
	}


	float xEntry{}, yEntry{}; 
	float xExit{}, yExit{}; 
	
	//Find time of leaving/time of collision
	if(b1.velocity.x < FLT_EPSILON && b1.velocity.x > -FLT_EPSILON)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / b1.velocity.x;
		xExit = xInvExit / b1.velocity.x;
	}

	
	if(b1.velocity.y < FLT_EPSILON && b1.velocity.y > -FLT_EPSILON)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / b1.velocity.y;
		yExit = yInvExit / b1.velocity.y;
	}


	if (xEntry > 1.0f) xEntry = -std::numeric_limits<float>::infinity();
	if (yEntry > 1.0f) yEntry = -std::numeric_limits<float>::infinity();

	float entryTime = std::max<float>(xEntry,yEntry);
	float exitTime = std::min<float>(xExit,yExit);


	//No Collision
	if(entryTime > exitTime)
	{
		return 1.f;
	}

	if(xEntry < 0.f && yEntry < 0.f)
	{
		return 1.f;
	}

	if(xEntry < 0.f)
	{
		if((b1.rect.x + b1.rect.w) < b2.rect.x || b1.rect.x > (b2.rect.x + b2.rect.w))
		{
			return 1.f;
		}
	}

	if(yEntry < 0.f)
	{
		if((b1.rect.y + b1.rect.h) < b2.rect.y || b1.rect.y > (b2.rect.y + b2.rect.y))
		{
			return 1.f;
		}
	}


	//Collision
	if(xEntry > yEntry)
	{
		if(xInvEntry < 0.f)
		{
			xNormal = 1.f;
			yNormal = 0.f;

			if(b1.rect.x < b2.rect.x)
				xNormal = -1.f;
		}
		else
		{
			xNormal = -1.f;
			yNormal = 0.f;

			if(b1.rect.x > b2.rect.x)
				xNormal = 1.f;
		}
	}
	else
	{
		if(yInvEntry < 0.f)
		{
			xNormal = 0.f;
			yNormal = 1.f;

			if(b1.rect.y < b2.rect.y)
				yNormal = -1.f;
		}
		else
		{
			xNormal = 0.f;
			yNormal = -1.f;

			if(b1.rect.y > b2.rect.y)
				yNormal = 1.f;
		}
	}

	return entryTime;

}

float crupt::CollisionSystem::SweptImproved(const Box& obj, const Box& other, eDirection& colDirection)
{
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
		}
		else
		{
			colDirection = eDirection::LEFT;
		}
	}
	else
	{
		if(dyEntry > 0.f)
		{
			colDirection = eDirection::UP;
		}
		else
		{
			colDirection = eDirection::DOWN;
		}
	}

	return entryTime;

}

Box crupt::CollisionSystem::GetSweptBroadphase(const Box& object)
{
	SDL_Rect result;
	result.x = object.velocity.x > 0.f ? int(object.rect.x) : int(object.rect.x + object.velocity.x);
	result.y = object.velocity.y > 0.f ? int(object.rect.y) : int(object.rect.y + object.velocity.y);
	result.w = int(object.rect.w + abs(object.velocity.x));
	result.h = int(object.rect.h + abs(object.velocity.y));

	Box res{};
	res.rect = result;

	return res;
}
