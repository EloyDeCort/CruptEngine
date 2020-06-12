#pragma once
#include "System.h"
#include "GameComponents.h"

namespace crupt
{
	class BubbleStateSystem : public System
	{
	public:
		virtual ~BubbleStateSystem();
		void Init();
		void OnDispatch(BubbleStateComponent component);

	private:
	};
}

