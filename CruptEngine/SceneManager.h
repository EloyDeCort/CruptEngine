#pragma once
#include "Singleton.h"
namespace crupt
{
	class GameScene;

	class SceneManager final  : public Singleton<SceneManager>
	{
	public:
		//Rule of 5
		SceneManager() = default;
		~SceneManager();
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) noexcept = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) noexcept = delete;

		void AddScene(GameScene* pScene);
		void RemoveScene(GameScene* pScene);
		void SetActiveScene(const std::wstring& sceneName);
		GameScene* GetActiveScene();
		GameScene* GetScene(const std::wstring& sceneName);
		const std::wstring& GetActiveSceneName() const;

		void Init();
		void Update(float dt);
		void FixedUpdate(float dt);
		void Render();

	private:

		std::vector<GameScene*> m_pScenes;
		GameScene* m_ActiveScene;
		GameScene* m_NewActiveScene;
	};
}