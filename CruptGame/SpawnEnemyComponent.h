#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace crupt
{
	//State of enemy
	enum class EnemyType
	{
		ZENCHAN,
		MAITA
	};

	//SIGNAL COMPONENT to spawn an enemy
	struct SpawnEnemyComponent
	{
		EnemyType type = EnemyType::ZENCHAN;
		glm::vec2 spawnPos = {0.f,0.f};
		bool flip = false;
	};
}