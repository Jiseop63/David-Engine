#pragma once
#include "daScene.h"

namespace da
{
	class Scene_Dungeon1F_Stage3 : public Scene
	{
	public:
		Scene_Dungeon1F_Stage3();
		virtual ~Scene_Dungeon1F_Stage3();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		void addBackgroundObjects();
		void addGameObjects();
	private:
	};
}