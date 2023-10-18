#pragma once
#include "daScene.h"

namespace da
{
	class Scene_dungeon2_boss : public Scene
	{
	public:
		Scene_dungeon2_boss();
		virtual ~Scene_dungeon2_boss();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		void addBackgroundObjects();
		void addGameObjects();
	};
}