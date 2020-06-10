#pragma once
#include "System.h"
#include "ECSCoordinator.h"
#include "Texture2D.h"

namespace crupt
{
	class PhysicsSystem : public System
	{
	public:
		virtual ~PhysicsSystem();
		void Init();
		void Update(float dt);

	private:
		ECSCoordinator* m_pCoordinator;
	};
}

