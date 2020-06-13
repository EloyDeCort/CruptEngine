#include "GameCommands.h"
#include "Components.h"
#include "GameComponents.h"
#include "ECSCoordinator.h"
#include "SignalHandler.h"

crupt::JumpCommand::JumpCommand(Entity pReceiver)
	: ICommand{"JumpCommand"}
	, m_pReceiver{pReceiver}
{
	
}

crupt::JumpCommand::~JumpCommand()
{
}

void crupt::JumpCommand::Execute()
{
	JumpComponent jumpComp;
	jumpComp.m_Target = m_pReceiver;

	SignalHandler<JumpComponent>::GetInstance().Publish(jumpComp);
}

crupt::MoveLeftCommand::MoveLeftCommand(Entity pReceiver)
	: ICommand{"MoveLeftCommand"}
	, m_pReceiver{pReceiver}
{
}

crupt::MoveLeftCommand::~MoveLeftCommand()
{
}

void crupt::MoveLeftCommand::Execute()
{
	MoveComponent moveComp;
	moveComp.m_Target = m_pReceiver;
	moveComp.m_Speed = 200.f;
	moveComp.m_xDirection = -1.f;
	
	SignalHandler<MoveComponent>::GetInstance().Publish(moveComp);
}
	
crupt::MoveRightCommand::MoveRightCommand(Entity pReceiver)
	: ICommand{"MoveRightCommand"}
	, m_pReceiver{pReceiver}
{	
}

crupt::MoveRightCommand::~MoveRightCommand()
{
}

void crupt::MoveRightCommand::Execute()
{
	MoveComponent moveComp;

	moveComp.m_Target = m_pReceiver;
	moveComp.m_Speed = 200.f;
	moveComp.m_xDirection = 1.f;

	SignalHandler<MoveComponent>::GetInstance().Publish(moveComp);
}

crupt::SpawnBubbleCommand::SpawnBubbleCommand(Entity pPlayer)
	: ICommand{"SpawnBubbleCommand"}
	, m_pPlayer{pPlayer}
	, m_Offset{40.f}
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	m_pState = &coordinator->GetComponent<PlayerStateComponent>(m_pPlayer);
}

crupt::SpawnBubbleCommand::~SpawnBubbleCommand()
{
}

void crupt::SpawnBubbleCommand::Execute()
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	const TransformComponent& playerTransComp = coordinator->GetComponent<TransformComponent>(m_pPlayer);
	const RenderableComponent& renderable = coordinator->GetComponent<RenderableComponent>(m_pPlayer);
	
	BubbleComponent bubbleComp;
	bubbleComp.position = playerTransComp.position;

	if(renderable.m_Flip)
	{
		bubbleComp.position.x -= m_Offset;
		bubbleComp.flipped = true;
	}
	else
	{
		bubbleComp.position.x += m_Offset;
		bubbleComp.flipped = false;
	}

	m_pState->m_AnimationState = PlayerAnimState::SPITBUBBLE;
	SignalHandler<BubbleComponent>::GetInstance().Publish(bubbleComp);
}
