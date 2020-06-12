#include "CruptEnginePCH.h"
#include "BubbleSpawnSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"
#include "ResourceManager.h"

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
	coordinator.AddComponent<RenderableComponent>(bubble, RenderableComponent{ResourceManager::GetInstance().LoadTexture("Items/Green_Bubble.png",m_pRenderer)});
	coordinator.AddComponent<SpriteComponent>(bubble, sprite);
	coordinator.AddComponent<MovePhysicsComponent>(bubble, MovePhysicsComponent{});
	
	//Reusing bubble component as tag
	coordinator.AddComponent<BubbleComponent>(bubble, BubbleComponent{component.position});
	coordinator.AddComponent<BoxCollisionComponent>(bubble, BoxCollisionComponent{0,0,32,32});
	coordinator.AddComponent<TransformComponent>(bubble, TransformComponent{component.position});
}
