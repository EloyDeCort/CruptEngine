#pragma once
#include <string>
#include "ECSCoordinator.h"
#include "Systems.h"
#include <set>

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
		bool OwnsEntity(Entity entity) const;
		void AddEntity(Entity entity);

		virtual void Init() = 0;
		virtual void Update(float dt) = 0;
		virtual void PostUpdate(float dt) = 0;
		virtual void Render() = 0;

	private:
		std::wstring m_SceneName;
		//Entities in this scene
		std::set<Entity> m_Entities;
	};
}
