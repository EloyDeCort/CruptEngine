#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)


namespace crupt
{
	//Easy enum class to check what player has fired the bubble
	enum class PlayerType
	{
		PLAYER1,
		PLAYER2
	};

	//SIGNAL COMPONENT
	//Later used as normal component (after spawn)
	struct BubbleComponent
	{
		glm::vec2 position;
		float totalTime = 0.f;
		float maxLifeTime = 5.f;
		float xSpeed = 200.f;
		float xSpeedTime = 0.5f;
		bool flipped = false;

		PlayerType type = PlayerType::PLAYER1;

		bool shouldPop = false;
	};
}