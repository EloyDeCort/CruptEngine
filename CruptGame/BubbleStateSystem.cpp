#include "BubbleStateSystem.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"

crupt::BubbleStateSystem::~BubbleStateSystem()
{
}

void crupt::BubbleStateSystem::Init()
{
	SignalHandler<BubbleStateComponent>::GetInstance().AddListener(std::bind(&crupt::BubbleStateSystem::OnDispatch, this, std::placeholders::_1));
}

void crupt::BubbleStateSystem::OnDispatch(BubbleStateComponent component)
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();

	SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(component.target);
	RenderableComponent& renderable = coordinator->GetComponent<RenderableComponent>(component.target);
	BubbleStateComponent& bubbleStateComp = coordinator->GetComponent<BubbleStateComponent>(component.target);

	bubbleStateComp.m_AnimationState = component.m_AnimationState;
	StateSprite& newSprite = bubbleStateComp.stateSprites[int(component.m_AnimationState)];
	spriteComp.m_AnimationRate = newSprite.m_SpriteData.m_AnimationRate;
	spriteComp.m_FrameCount = newSprite.m_SpriteData.m_FrameCount;
	renderable.m_Texture = bubbleStateComp.stateSprites[int(component.m_AnimationState)].m_Texture;
}
