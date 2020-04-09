#pragma once
#include "CruptEngine.h"
namespace crupt
{
	class MainGame : public CruptEngine
	{
	public:
		virtual void InitGame() override;
	private:
		void RegisterAdditionalComponents();
		void RegisterAdditionalSystems();

	};
}

