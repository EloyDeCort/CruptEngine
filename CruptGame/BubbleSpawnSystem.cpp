#include "CruptEnginePCH.h"
#include "BubbleSpawnSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"
#include "ResourceManager.h"
#include "GameSystems.h"

crupt::BubbleSpawnSystem::~BubbleSpawnSystem()
{
	
}

void crupt::BubbleSpawnSystem::Init(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
	SignalHandler<BubbleComponent>::GetInstance().AddListener(std::bind(&crupt::BubbleSpawnSystem::OnDispatch, this, std::placeholders::_1));

}

void crupt::BubbleSpawnSystem::OnDispatch(BubbleComponent component)
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();

	Entity bubble = coordinator.CreateEntity();

	SpriteComponent sprite;
	sprite.m_FrameCount = 1;
	sprite.m_ScaleFactor = 2;
	coordinator.AddComponent<RenderableComponent>(bubble,RenderableComponent{ResourceManager::GetInstance().LoadTexture("Items/Green_Bubble.png",m_pRenderer)});
	coordinator.AddComponent<SpriteComponent>(bubble, sprite);
	coordinator.AddComponent<MovePhysicsComponent>(bubble, MovePhysicsComponent{});
	
	//Reusing bubble component as tag
	coordinator.AddComponent<BubbleComponent>(bubble, component);
	BoxCollisionComponent col; 
	col.m_CollisionRect = SDL_Rect{0,0,32,32};
	col.m_IgnorePlatforms = true;
	col.m_IgnoreEntities = true;
	coordinator.AddComponent<BoxCollisionComponent>(bubble,col);
	coordinator.AddComponent<TransformComponent>(bubble, TransformComponent{component.position});
	coordinator.AddComponent<CollisionCallbackComponent>(bubble, CollisionCallbackComponent{});

	coordinator.GetSystem<CollisionCallbackSystem>()->AddEntityCallback(bubble);
}
