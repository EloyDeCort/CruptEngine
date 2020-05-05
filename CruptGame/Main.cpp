#include "CruptEnginePCH.h"
#include "MainGame.h"
#include "SDL.h"

int main(int, char*[]) {

	crupt::MainGame* engine = new crupt::MainGame();

	//TODO: Remove this try,catch after a time and make a cleaner solution
	try
	{
		engine->Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "[EXCEPTION THROWN]: " << e.what() << std::endl;
	}
	delete engine;
    return 0;
}