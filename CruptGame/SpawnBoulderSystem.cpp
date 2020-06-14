#include "CruptEnginePCH.h"
#include "SpawnBoulderSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"
#include "ResourceManager.h"
#include "GameSystems.h"

crupt::SpawnBoulderSystem::~SpawnBoulderSystem()
{
}

void crupt::SpawnBoulderSystem::Init(SDL_Renderer* renderer, CollisionCallbackSystem* colCallbackSystem)
{
	m_pRenderer = renderer;
	m_pCollisionCallbackSystem = colCallbackSystem;
	SignalHandler<BoulderComponent>::GetInstance().AddListener(std::bind(&crupt::SpawnBoulderSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::SpawnBoulderSystem::PreUpdate(float dt)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	std::vector<Entity> toDelete;
	for (Entity entity : m_Entities)
	{
		BoulderComponent& boulderComp = coordinator->GetComponent<BoulderComponent>(entity);
		BoxCollisionComponent& boxComp = coordinator->GetComponent<BoxCollisionComponent>(entity);
		MovePhysicsComponent& movPhysicsComp = coordinator->GetComponent<MovePhysicsComponent>(entity);

		boulderComp.totalTime += dt;

		if(boulderComp.totalTime > boulderComp.maxAliveTime)
		{
			boulderComp.shouldDestroy = true;
		}

		if(boulderComp.shouldDestroy)
		{
			toDelete.push_back(entity);
			continue;
		}		

		if(boxComp.colDirY == eDirection::DOWN)
		{
			if(!boulderComp.flipped)
			{
				movPhysicsComp.force.x = boulderComp.speed;
			}
			else
			{
				movPhysicsComp.force.x = -boulderComp.speed;
			}

		}
	}

	for (size_t i{}; i < toDelete.size(); ++i)
	{
		coordinator->DestroyEntity(toDelete[i]);
	}
}

void crupt::SpawnBoulderSystem::OnDispatch(const BoulderComponent& component)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Enemies/Boulder_Roll.png",m_pRenderer);

	SpriteComponent spriteComp{};
	spriteComp.animationRate = 12; 
	spriteComp.scaleFactor = 2; 
	spriteComp.frameCount = 4; 

	Entity boulder = coordinator->CreateEntity();

	RenderableComponent renderComp = RenderableComponent{defaultAnim};
	renderComp.flip = component.flipped;

	coordinator->AddComponent<SpriteComponent>(boulder, spriteComp);
	coordinator->AddComponent<EntityComponent>(boulder, EntityComponent{});
	coordinator->AddComponent<RenderableComponent>(boulder, renderComp);


	float xPos = component.pos.x;
	if(component.flipped)
	{
		xPos -= 20.f;
	}
	else
	{
		xPos += 20.f;
	}

	coordinator->AddComponent<TransformComponent>(boulder, TransformComponent{{xPos, component.pos.y}});
	coordinator->AddComponent<MovePhysicsComponent>(boulder, MovePhysicsComponent{});
	coordinator->AddComponent<GravityComponent>(boulder, GravityComponent{});
	coordinator->AddComponent<BoxCollisionComponent>(boulder, BoxCollisionComponent{0,0,32,32});
	coordinator->AddComponent<CollisionCallbackComponent>(boulder, CollisionCallbackComponent{});
	BoulderComponent boulderComp = BoulderComponent{};
	boulderComp.pos = component.pos;
	boulderComp.flipped = component.flipped;

	coordinator->AddComponent<BoulderComponent>(boulder, boulderComp);
	m_pCollisionCallbackSystem->AddEntityCallback(boulder);	
}
