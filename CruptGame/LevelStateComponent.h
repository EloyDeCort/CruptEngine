#pragma once

namespace crupt
{
	//SIGNAL COMPONENT to ensure proper level swapping
	struct LevelStateComponent
	{
		int currentLevel = 0;
		bool nextLevel = false;
		bool endGame = false;
	};
}

