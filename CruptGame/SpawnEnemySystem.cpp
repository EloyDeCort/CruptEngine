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
	m_pRenderer = renderer;
	m_pCollisionCallbackSystem = colCallbackSystem;
	SignalHandler<SpawnEnemyComponent>::GetInstance().AddListener(std::bind(&crupt::SpawnEnemySystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::SpawnEnemySystem::OnDispatch(SpawnEnemyComponent component)
{
	if(component.type == EnemyType::ZENCHAN)
	{
		SpawnZenchan(component.spawnPos);
	}
}

void crupt::SpawnEnemySystem::SetPlayer1(Entity player)
{
	m_Player1 = player;
}

void crupt::SpawnEnemySystem::SpawnZenchan(const glm::vec2& pos)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	//ZenChan
	SpriteComponent spriteComp{};
	spriteComp.m_AnimationRate = 12; 
	spriteComp.m_ScaleFactor = 2; 
	spriteComp.m_FrameCount = 8; 
	Texture2D* defaultAnim = ResourceManager::GetInstance().LoadTexture("Enemies/ZenChan_Walking.png",m_pRenderer);

	Entity zenchanEnemy = coordinator->CreateEntity();
	coordinator->AddComponent<SpriteComponent>(zenchanEnemy, spriteComp);
	coordinator->AddComponent<RenderableComponent>(zenchanEnemy, RenderableComponent{defaultAnim});
	coordinator->AddComponent<TransformComponent>(zenchanEnemy, TransformComponent{pos});
	coordinator->AddComponent<MovePhysicsComponent>(zenchanEnemy, MovePhysicsComponent{});
	coordinator->AddComponent<GravityComponent>(zenchanEnemy, GravityComponent{});
	coordinator->AddComponent<BoxCollisionComponent>(zenchanEnemy, BoxCollisionComponent{0,0,32,32});
	coordinator->AddComponent<CollisionCallbackComponent>(zenchanEnemy, CollisionCallbackComponent{});
	ZenchanComponent zenchanComp = ZenchanComponent{};
	zenchanComp.player1 = m_Player1;
	coordinator->AddComponent<ZenchanComponent>(zenchanEnemy, zenchanComp);
	m_pCollisionCallbackSystem->AddEntityCallback(zenchanEnemy);
}
