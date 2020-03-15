#include "CruptEnginePCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "CruptEngine.h"

int main(int, char*[]) {
	dae::CruptEngine engine;
	engine.Run();
    return 0;
}