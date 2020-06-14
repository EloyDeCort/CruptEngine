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
	sprite.frameCount = 1;
	sprite.scaleFactor = 2;
	sprite.frameCount = 1;
	m_BubbleStateCompP1.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Green_Bubble.png",m_pRenderer)});
	m_BubbleStateCompP2.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Blue_Bubble.png",m_pRenderer)});
	
	sprite.frameCount = 8;
	m_BubbleStateCompP1.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Green_Bubble_Zenchan.png",m_pRenderer)});
	m_BubbleStateCompP2.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Blue_Bubble_Zenchan.png",m_pRenderer)});

	m_BubbleStateCompP1.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Green_Bubble_Maita.png",m_pRenderer)});
	m_BubbleStateCompP2.stateSprites.push_back(StateSprite{sprite,ResourceManager::GetInstance().LoadTexture("Items/Blue_Bubble_Maita.png",m_pRenderer)});

	m_BubbleStateCompP1.animationState = BubbleAnimState::NORMAL;
	m_BubbleStateCompP2.animationState = BubbleAnimState::NORMAL;

	m_BoxCollisionComp.collisionRect = SDL_Rect{0,0,32,32};
	m_BoxCollisionComp.ignorePlatforms = true;
	m_BoxCollisionComp.ignoreEntities = true;
}

void crupt::BubbleSpawnSystem::OnDispatch(const BubbleComponent& component)
{
	ECSCoordinator& coordinator = ECSCoordinator::GetInstance();

	Entity bubble = coordinator.CreateEntity();

	switch(component.type)
	{
	case PlayerType::PLAYER1:
			coordinator.AddComponent<RenderableComponent>(bubble,RenderableComponent{m_BubbleStateCompP1.stateSprites[0].pTexture});
	
			coordinator.AddComponent<SpriteComponent>(bubble, m_BubbleStateCompP1.stateSprites[0].spriteData);

			coordinator.AddComponent<BubbleStateComponent>(bubble, m_BubbleStateCompP1);
		break;

	case PlayerType::PLAYER2:
			coordinator.AddComponent<RenderableComponent>(bubble,RenderableComponent{m_BubbleStateCompP2.stateSprites[0].pTexture});
	
			coordinator.AddComponent<SpriteComponent>(bubble, m_BubbleStateCompP2.stateSprites[0].spriteData);

			coordinator.AddComponent<BubbleStateComponent>(bubble, m_BubbleStateCompP2);
		break;
	}

	coordinator.AddComponent<MovePhysicsComponent>(bubble, MovePhysicsComponent{});
	
	//Reusing bubble component as tag
	coordinator.AddComponent<BubbleComponent>(bubble, component);


	coordinator.AddComponent<BoxCollisionComponent>(bubble,m_BoxCollisionComp);
	coordinator.AddComponent<TransformComponent>(bubble, TransformComponent{component.position});
	coordinator.AddComponent<CollisionCallbackComponent>(bubble, CollisionCallbackComponent{});

	coordinator.GetSystem<CollisionCallbackSystem>()->AddEntityCallback(bubble);
}
