#pragma once
#include "ECSTypes.h"
#include <set>

namespace crupt
{
	class System
	{
	public:
		std::set<Entity> m_Entities;
	};
}

