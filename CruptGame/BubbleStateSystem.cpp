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

	bubbleStateComp.animationState = component.animationState;
	StateSprite& newSprite = bubbleStateComp.stateSprites[int(component.animationState)];
	spriteComp.animationRate = newSprite.spriteData.animationRate;
	spriteComp.frameCount = newSprite.spriteData.frameCount;
	renderable.pTexture = bubbleStateComp.stateSprites[int(component.animationState)].pTexture;
}
