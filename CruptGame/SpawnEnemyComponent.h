#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace crupt
{
	enum class EnemyType
	{
		ZENCHAN,
		MAITA
	};

	//SIGNAL COMPONENT
	struct SpawnEnemyComponent
	{
		EnemyType type = EnemyType::ZENCHAN;
		glm::vec2 spawnPos = {0.f,0.f};
		bool flip = false;
	};
}