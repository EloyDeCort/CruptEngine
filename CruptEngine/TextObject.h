#pragma once
#include "GameObject.h"

namespace crupt
{
	class Font;
	class Texture2D;
	class TextObject final : public GameObject
	{
	public:
		void Update();
		void Render() const;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

		explicit TextObject(const std::string& text, Font* font);
		virtual ~TextObject();
		TextObject(const TextObject& other) = delete;
		TextObject(TextObject&& other) = delete;
		TextObject& operator=(const TextObject& other) = delete;
		TextObject& operator=(TextObject&& other) = delete;
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		//Transform m_Transform;
		Font* m_pFont;
		Texture2D* m_pTexture;
	};
}
