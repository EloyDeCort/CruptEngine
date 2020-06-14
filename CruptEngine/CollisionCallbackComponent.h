#pragma once
#include <functional>
#include "ECSCoordinator.h"

namespace crupt
{
	//Component that acts as delegate.
	struct CollisionCallbackComponent
	{
		//Delegate
		using OnCollision = std::function<void (Entity self, Entity collider, eDirection direction)>;

		OnCollision onCollision = nullptr;
	};
}