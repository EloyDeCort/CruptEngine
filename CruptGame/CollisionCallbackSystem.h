#pragma once
#include "System.h"
#include "GameComponents.h"
#include "Components.h"

namespace crupt
{
	class ECSCoordinator;
	class CollisionCallbackSystem : public System
	{
	public:
		virtual ~CollisionCallbackSystem();
		void Init();
		void AddEntityCallback(Entity entity);
		void OnPlayerCallback(Entity self, Entity collider, eDirection direction);
		void OnBubbleCallback(Entity self, Entity collider, eDirection direction);

	private:
		ECSCoordinator* m_pCoordinator;
	};
}

