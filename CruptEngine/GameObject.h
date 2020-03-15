#pragma once

namespace crupt
{
	class Texture2D;
	class GameObject
	{
	public:
		void Update();
		void Render() const;

		void SetTexture(const std::string& filename)	;
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:

		//Transform m_Transform;
		//Texture2D* m_pTexture;
	};
}
