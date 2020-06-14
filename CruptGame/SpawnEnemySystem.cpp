#include "SpawnEnemySystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"
#include "ResourceManager.h"
#include "GameSystems.h"

crupt::SpawnEnemySystem::~SpawnEnemySystem()
{
}

void crupt::SpawnEnemySystem::Init(SDL_Renderer* renderer, CollisionCallbackSystem* colCallbackSystem)
{
	m_NrOfPlayers = 0;
	m_pRenderer = renderer;
	m_pCollisionCallbackSystem = colCallbackSystem;
	SignalHandler<SpawnEnemyComponent>::GetInstance().AddListener(std::bind(&crupt::SpawnEnemySystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::SpawnEnemySystem::OnDispatch(const SpawnEnemyComponent& component)
{
	if(component.type == EnemyType::ZENCHAN)
	{
		SpawnZenchan(component);
	}
	else if (component.type == EnemyType::MAITA)
	{
		SpawnMaita(component);
	}
}

void crupt::SpawnEnemySystem::SetPlayer1(Entity player)
{
	m_Player1 = player;
	m_NrOfPlayers++;
}

void crupt::SpawnEnemySystem::SetPlayer2(Entity player)
{
	m_Player2 = player;
	m_NrOfPlayers++;
}

void crupt::SpawnEnemySystem::Reset()
{
	m_NrOfPlayers = 0;
}

void crupt::SpawnEnemySystem::SpawnZenchan(const SpawnEnemyComponent& component)
{
	//Spawning a new zen chan
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	//ZenChan
	SpriteComponent spriteComp{};
	spriteComp.animationRate = 12; 
	spriteComp.scaleFactor = 2; 
	spriteComp.frameCount = 8; 
	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Enemies/ZenChan_Walking.png",m_pRenderer);

	Entity zenchanEnemy = coordinator->CreateEntity();
	RenderableComponent renderComp = RenderableComponent{defaultAnim};
	renderComp.flip = component.flip;
	coordinator->AddComponent<SpriteComponent>(zenchanEnemy, spriteComp);
	coordinator->AddComponent<EntityComponent>(zenchanEnemy, EntityComponent{});
	coordinator->AddComponent<EnemyComponent>(zenchanEnemy, EnemyComponent{});
	coordinator->AddComponent<RenderableComponent>(zenchanEnemy, renderComp);
	coordinator->AddComponent<TransformComponent>(zenchanEnemy, TransformComponent{component.spawnPos});
	coordinator->AddComponent<MovePhysicsComponent>(zenchanEnemy, MovePhysicsComponent{});
	coordinator->AddComponent<GravityComponent>(zenchanEnemy, GravityComponent{});
	coordinator->AddComponent<BoxCollisionComponent>(zenchanEnemy, BoxCollisionComponent{0,0,32,32});
	coordinator->AddComponent<CollisionCallbackComponent>(zenchanEnemy, CollisionCallbackComponent{});
	ZenchanComponent zenchanComp = ZenchanComponent{};
	zenchanComp.player1 = m_Player1;
	zenchanComp.flipped = component.flip;
	if(m_NrOfPlayers >= 2)
	{
		zenchanComp.player2 = m_Player2;
		zenchanComp.coOp = true;
	}
	coordinator->AddComponent<ZenchanComponent>(zenchanEnemy, zenchanComp);
	m_pCollisionCallbackSystem->AddEntityCallback(zenchanEnemy);
}

void crupt::SpawnEnemySystem::SpawnMaita(const SpawnEnemyComponent& component)
{
	//Spawning a new maita
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	//Maita
	MaitaComponent maitaComp = MaitaComponent{};
	maitaComp.state = MaitaAnimState::WALKING;

	SpriteComponent spriteComp{};
	spriteComp.animationRate = 12; 
	spriteComp.scaleFactor = 2; 
	spriteComp.frameCount = 5; 
	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Enemies/Maita_Walking.png",m_pRenderer);

	Entity maitaEnemy = coordinator->CreateEntity();
	RenderableComponent renderComp = RenderableComponent{defaultAnim};
	renderComp.flip = component.flip;
	coordinator->AddComponent<SpriteComponent>(maitaEnemy, spriteComp);
	coordinator->AddComponent<RenderableComponent>(maitaEnemy, renderComp);

	maitaComp.pStateSprites.push_back(StateSprite{spriteComp,defaultAnim});
	spriteComp.animationRate = 5; 
	spriteComp.frameCount = 5; 
	maitaComp.pStateSprites.push_back(StateSprite{spriteComp,ResourceManager::GetInstance().LoadTexture("Enemies/Maita_Charging.png",m_pRenderer)});

	coordinator->AddComponent<EntityComponent>(maitaEnemy, EntityComponent{});
	coordinator->AddComponent<EnemyComponent>(maitaEnemy, EnemyComponent{});
	coordinator->AddComponent<TransformComponent>(maitaEnemy, TransformComponent{component.spawnPos});
	coordinator->AddComponent<MovePhysicsComponent>(maitaEnemy, MovePhysicsComponent{});
	coordinator->AddComponent<GravityComponent>(maitaEnemy, GravityComponent{});
	coordinator->AddComponent<BoxCollisionComponent>(maitaEnemy, BoxCollisionComponent{0,0,32,32});
	coordinator->AddComponent<CollisionCallbackComponent>(maitaEnemy, CollisionCallbackComponent{});
	
	maitaComp.player1 = m_Player1;
	maitaComp.flipped = component.flip;
	
	if(m_NrOfPlayers >= 2)
	{
		maitaComp.player2 = m_Player2;
		maitaComp.coOp = true;
	}

	coordinator->AddComponent<MaitaComponent>(maitaEnemy, maitaComp);
	m_pCollisionCallbackSystem->AddEntityCallback(maitaEnemy);
}

