#pragma once
#include "System.h"
#include <unordered_map>
#include <string>
#include "Texture2D.h"

namespace crupt
{
	class GraphicsSystem : public System
	{
	public:
		virtual ~GraphicsSystem();
		void Render();
	private:
		
	};
}

