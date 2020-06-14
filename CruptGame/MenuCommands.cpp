#include "CruptEnginePCH.h"
#include "MenuCommands.h"
#include "BBMainMenuScene.h"
#include "BubbleBobbleScene.h"
#include "SceneManager.h"

crupt::SingleCommand::SingleCommand()
	: ICommand("SingleCommand")
{
}

crupt::SingleCommand::~SingleCommand()
{
}

void crupt::SingleCommand::Execute()
{
	BubbleBobbleScene* bubBobScene = reinterpret_cast<BubbleBobbleScene*>(SceneManager::GetInstance().GetScene(L"BubbleBobbleScene"));
	bubBobScene->SetGameMode(GameMode::SINGLE);

	SceneManager::GetInstance().SetActiveScene(L"BubbleBobbleScene");
}

crupt::CoOpCommand::CoOpCommand()
	: ICommand("CoOpCommand")
{
}

crupt::CoOpCommand::~CoOpCommand()
{
}

void crupt::CoOpCommand::Execute()
{
	BubbleBobbleScene* bubBobScene = reinterpret_cast<BubbleBobbleScene*>(SceneManager::GetInstance().GetScene(L"BubbleBobbleScene"));
	bubBobScene->SetGameMode(GameMode::COOP);

	SceneManager::GetInstance().SetActiveScene(L"BubbleBobbleScene");
}

crupt::ToMainMenuCommand::ToMainMenuCommand()
	: ICommand("ToMainMenuCommand")
{
}

crupt::ToMainMenuCommand::~ToMainMenuCommand()
{
}

void crupt::ToMainMenuCommand::Execute()
{
	SceneManager::GetInstance().SetActiveScene(L"BBMainMenuScene");
}
