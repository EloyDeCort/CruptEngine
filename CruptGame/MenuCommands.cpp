#include "MenuCommands.h"
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
	SceneManager::GetInstance().SetActiveScene(L"BubbleBobbleScene");
}
