#pragma once
#include "ECSTypes.h"
#include <set>

namespace crupt
{
	class System
	{
	public:
		virtual ~System() = default;
		std::set<Entity> m_Entities;
	};
}

