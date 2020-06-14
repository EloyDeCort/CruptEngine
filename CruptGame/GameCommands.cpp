#include "CruptEnginePCH.h"
#include "GameCommands.h"
#include "Components.h"
#include "GameComponents.h"
#include "ECSCoordinator.h"
#include "SoundManager.h"
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
	jumpComp.target = m_pReceiver;

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
	moveComp.target = m_pReceiver;
	moveComp.speed = 200.f;
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

	moveComp.target = m_pReceiver;
	moveComp.speed = 200.f;
	moveComp.m_xDirection = 1.f;

	SignalHandler<MoveComponent>::GetInstance().Publish(moveComp);
}

crupt::SpawnBubbleCommand::SpawnBubbleCommand(Entity pPlayer, PlayerType type, bool maita)
	: ICommand{"SpawnBubbleCommand"}
	, m_pPlayer{pPlayer}
	, m_Offset{40.f}
	, m_PlayerType{type}
	, m_IsMaita{maita}
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	if(m_IsMaita)
	{
		m_pMaitaComp = &coordinator->GetComponent<MaitaComponent>(m_pPlayer);
	}
	else
	{
		m_pState = &coordinator->GetComponent<PlayerStateComponent>(m_pPlayer);
	}
}

crupt::SpawnBubbleCommand::~SpawnBubbleCommand()
{
}

void crupt::SpawnBubbleCommand::Execute()
{
	ECSCoordinator* coordinator = &ECSCoordinator::GetInstance();
	const TransformComponent& playerTransComp = coordinator->GetComponent<TransformComponent>(m_pPlayer);
	RenderableComponent& renderable = coordinator->GetComponent<RenderableComponent>(m_pPlayer);
	
	if(m_IsMaita)
	{
		//Check if the player is a maita (VERSUS)
		MaitaComponent& maitaComp = coordinator->GetComponent<MaitaComponent>(m_pPlayer);
		/*if(maitaComp.totalTime < maitaComp.chargeCoolDown)
		{
			return;
		}*/

		SpriteComponent& spriteComp = coordinator->GetComponent<SpriteComponent>(m_pPlayer);
		BoulderComponent boulderComp;
		boulderComp.flipped = renderable.flip;
		boulderComp.pos = playerTransComp.position;
		boulderComp.fromPlayer = true;
		boulderComp.player2 = m_pPlayer;
		SignalHandler<BoulderComponent>::GetInstance().Publish(boulderComp);


		maitaComp.state = MaitaAnimState::CHARGING;

		//Change Sprite
		StateSprite& newSprite = maitaComp.pStateSprites[int(maitaComp.state)];
		spriteComp.animationRate = newSprite.spriteData.animationRate;
		spriteComp.frameCount = newSprite.spriteData.frameCount;
		renderable.pTexture = maitaComp.pStateSprites[int(maitaComp.state)].pTexture;

		maitaComp.totalTime = 0.f;

		return;
	}

	//If not in versus mode, spawn a bubble.
	BubbleComponent bubbleComp;
	bubbleComp.position = playerTransComp.position;
	bubbleComp.type = m_PlayerType;

	if(renderable.flip)
	{
		bubbleComp.position.x -= m_Offset;
		bubbleComp.flipped = true;
	}
	else
	{
		bubbleComp.position.x += m_Offset;
		bubbleComp.flipped = false;
	}

	m_pState->animationState = PlayerAnimState::SPITBUBBLE;
	SignalHandler<BubbleComponent>::GetInstance().Publish(bubbleComp);
}
