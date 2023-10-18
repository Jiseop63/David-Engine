#pragma once
#include "daScene.h"

namespace da
{
	class Scene_dungeon1_final : public Scene
	{
	public:
		Scene_dungeon1_final();
		virtual ~Scene_dungeon1_final();
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