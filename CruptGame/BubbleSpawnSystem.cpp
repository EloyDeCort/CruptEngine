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
	
	SpriteComponent sprite;
	sprite.m_FrameCount = 1;
	sprite.m_ScaleFactor = 2;
	sprite.m_FrameCount = 1;
	m_BubbleStateComp.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Green_Bubble.png",m_pRenderer)});
	
	sprite.m_FrameCount = 8;
	m_BubbleStateComp.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Bubble_Maita.png",m_pRenderer)});

	m_BubbleStateComp.m_AnimationState = BubbleAnimState::NORMAL;


	m_BoxCollisionComp.m_CollisionRect = SDL_Rect{0,0,32,32};
	m_BoxCollisionComp.m_IgnorePlatforms = true;
	m_BoxCollisionComp.m_IgnoreEntities = true;
}

void crupt::BubbleSpawnSystem::OnDispatch(BubbleComponent component)
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();

	Entity bubble = coordinator.CreateEntity();

	coordinator.AddComponent<RenderableComponent>(bubble,RenderableComponent{m_BubbleStateComp.stateSprites[0].m_Texture});

	coordinator.AddComponent<SpriteComponent>(bubble, m_BubbleStateComp.stateSprites[0].m_SpriteData);

	coordinator.AddComponent<BubbleStateComponent>(bubble, m_BubbleStateComp);

	coordinator.AddComponent<MovePhysicsComponent>(bubble, MovePhysicsComponent{});
	
	//Reusing bubble component as tag
	coordinator.AddComponent<BubbleComponent>(bubble, component);

	coordinator.AddComponent<BoxCollisionComponent>(bubble,m_BoxCollisionComp);
	coordinator.AddComponent<TransformComponent>(bubble, TransformComponent{component.position});
	coordinator.AddComponent<CollisionCallbackComponent>(bubble, CollisionCallbackComponent{});

	coordinator.GetSystem<CollisionCallbackSystem>()->AddEntityCallback(bubble);
}
