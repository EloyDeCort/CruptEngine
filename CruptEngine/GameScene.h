#pragma once
#include <string>
#include "ECSCoordinator.h"
#include "Systems.h"

namespace crupt
{
	//virtual class, used to derive from to create scenes
	class GameScene
	{
	public:
		GameScene(const std::wstring& sceneName);
		//Rule of 5
		virtual ~GameScene();
		GameScene(const GameScene& other) = delete;
		GameScene(GameScene&& other) noexcept = delete;
		GameScene& operator=(const GameScene& other) = delete;
		GameScene& operator=(GameScene&& other) noexcept = delete;

		const std::wstring& GetSceneName() const;

		virtual void Init() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

	private:
		std::wstring m_SceneName;
	};
}
