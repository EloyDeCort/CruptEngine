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


		for(const SDL_Rect& collision : m_TileComp->m_SolidCollisionsMap.at(m_TileComp->m_CurrentLevel))
		{
			Box wallBox{};
			wallBox.rect = collision;
			wallBox.velocity = glm::vec2{0.f,0.f};

			float xNormal{0.f};
			float yNormal{0.f};

			float entryTime = SweptAABB(playerBox, wallBox, xNormal, yNormal);

			SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(m_pRenderer, &boxComp.m_CollisionRect);

			SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
			boxComp.m_EntryTime = entryTime;
			boxComp.m_IsGrounded = false;
			if(entryTime < 1.f)
			{
				if(yNormal < -0.01f)
				{
					boxComp.m_IsGrounded = true;
				}
				boxComp.m_Colliding = true;
				return;
			}
			else
			{
				boxComp.m_Colliding = false;
			}
				
		}

		/*for(const SDL_Rect& collision : m_TileComp->m_PlatformCollisionsMap.at(m_TileComp->m_CurrentLevel))
		{
			Box wallBox{};
			wallBox.rect = collision;
			wallBox.velocity = glm::vec2{0.f,0.f};

			float xNormal{0.f};
			float yNormal{0.f};

			float entryTime = SweptAABB(playerBox, wallBox, xNormal, yNormal);

			SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(m_pRenderer, &boxComp.m_CollisionRect);

			SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
			boxComp.m_EntryTime = entryTime;
			boxComp.m_IsGrounded = false;
			if(entryTime < 1.f)
			{
				if(yNormal < -0.01f)
				{
					boxComp.m_IsGrounded = true;
				}
				boxComp.m_Colliding = true;
				return;
			}
			else
			{
				boxComp.m_Colliding = false;
			}
				
		}*/

	}
}

bool crupt::CollisionSystem::AABB(SDL_Rect b1, SDL_Rect b2)
{
	return (b1.x < b2.x + b2.w && 
		b1.x + b1.w > b2.x && 
		b1.y < b2.y + b2.h && 
		b1.y + b1.h > b2.y);
}

float crupt::CollisionSystem::SweptAABB(Box b1, Box b2, float& xNormal, float& yNormal)
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


	float entryTime = std::max<float>(xEntry,yEntry);
	float exitTime = std::min<float>(xExit,yExit);


	//No Collision
	if(entryTime > exitTime)
	{
		xNormal = 0.f;
		yNormal = 0.f;
		return 1.f;
	}

	if(xEntry < 0.f && yEntry < 0.f)
	{
		xNormal = 0.f;
		yNormal = 0.f;
		return 1.f;
	}

	if(xEntry < 0.f)
	{
		if((b1.rect.x + b1.rect.w) < b2.rect.x || b1.rect.x > (b2.rect.x + b2.rect.w))
		{
			xNormal = 0.f;
			yNormal = 0.f;
			return 1.f;
		}
	}

	if(yEntry < 0.f)
	{
		if((b1.rect.y + b1.rect.h) < b2.rect.y || b1.rect.y > (b2.rect.y + b2.rect.y))
		{
			xNormal = 0.f;
			yNormal = 0.f;
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
		}
		else
		{
			xNormal = -1.f;
			yNormal = 0.f;
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
