#pragma once
#include "System.h"
namespace crupt
{
	class TextSystem : public System
	{
	public:
		virtual ~TextSystem();
		void Render();
		void Update();
		void SetText(Entity& entity, const std::string& text);
	private:
	};
}

