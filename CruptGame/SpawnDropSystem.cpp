#include "SpawnDropSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"
#include "ResourceManager.h"
#include "GameSystems.h"

crupt::SpawnDropSystem::~SpawnDropSystem()
{
}

void crupt::SpawnDropSystem::Init(SDL_Renderer* renderer, CollisionCallbackSystem* colCallbackSystem)
{
	//SCORES
	m_MelonScore = 100;
	m_FriesScore = 200;
	m_MaxYOffset = 80.f;

	m_pRenderer = renderer;
	m_pCollisionCallbackSystem = colCallbackSystem;
	SignalHandler<DropComponent>::GetInstance().AddListener(std::bind(&crupt::SpawnDropSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::SpawnDropSystem::OnDispatch(const DropComponent& component)
{
	if(component.type == DropType::MELON)
	{
		SpawnMelon(component.pos);
	}
	else if(component.type == DropType::FRIES)
	{
		SpawnFries(component.pos);
	}
}

void crupt::SpawnDropSystem::SpawnMelon(const glm::vec2& pos)
{
	//Spawn a melon
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Drops/Melon.png",m_pRenderer);

	SpriteComponent spriteComp{};
	spriteComp.animationRate = 1; 
	spriteComp.scaleFactor = 2; 
	spriteComp.frameCount = 1; 

	Entity melonDrop = coordinator->CreateEntity();

	coordinator->AddComponent<SpriteComponent>(melonDrop, spriteComp);
	coordinator->AddComponent<EntityComponent>(melonDrop, EntityComponent{});
	coordinator->AddComponent<RenderableComponent>(melonDrop, RenderableComponent{defaultAnim});

	float yPos = pos.y;
	if(yPos > m_MaxYOffset)
	{
		//reduce by collision size
		yPos -= 32.f;
	}
	coordinator->AddComponent<TransformComponent>(melonDrop, TransformComponent{{pos.x, yPos}});
	coordinator->AddComponent<MovePhysicsComponent>(melonDrop, MovePhysicsComponent{});
	coordinator->AddComponent<GravityComponent>(melonDrop, GravityComponent{});
	coordinator->AddComponent<BoxCollisionComponent>(melonDrop, BoxCollisionComponent{0,0,32,32});
	coordinator->AddComponent<CollisionCallbackComponent>(melonDrop, CollisionCallbackComponent{});
	DropComponent dropComp = DropComponent{};
	dropComp.pos = pos;
	dropComp.score = m_MelonScore;
	dropComp.type = DropType::MELON;


	coordinator->AddComponent<DropComponent>(melonDrop, dropComp);
	m_pCollisionCallbackSystem->AddEntityCallback(melonDrop);
}

void crupt::SpawnDropSystem::SpawnFries(const glm::vec2& pos)
{
	//Spawn fries
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Drops/Fries.png",m_pRenderer);

	SpriteComponent spriteComp{};
	spriteComp.animationRate = 1; 
	spriteComp.scaleFactor = 2; 
	spriteComp.frameCount = 1; 

	Entity friesDrop = coordinator->CreateEntity();

	coordinator->AddComponent<SpriteComponent>(friesDrop, spriteComp);
	coordinator->AddComponent<EntityComponent>(friesDrop, EntityComponent{});
	coordinator->AddComponent<RenderableComponent>(friesDrop, RenderableComponent{defaultAnim});

	float yPos = pos.y;
	if(yPos > m_MaxYOffset)
	{
		//reduce by collision size
		yPos -= 32.f;
	}
	coordinator->AddComponent<TransformComponent>(friesDrop, TransformComponent{{pos.x, yPos}});
	coordinator->AddComponent<MovePhysicsComponent>(friesDrop, MovePhysicsComponent{});
	coordinator->AddComponent<GravityComponent>(friesDrop, GravityComponent{});
	coordinator->AddComponent<BoxCollisionComponent>(friesDrop, BoxCollisionComponent{0,0,32,32});
	coordinator->AddComponent<CollisionCallbackComponent>(friesDrop, CollisionCallbackComponent{});
	DropComponent dropComp = DropComponent{};
	dropComp.pos = pos;
	dropComp.score = m_FriesScore;
	dropComp.type = DropType::FRIES;


	coordinator->AddComponent<DropComponent>(friesDrop, dropComp);
	m_pCollisionCallbackSystem->AddEntityCallback(friesDrop);
}
