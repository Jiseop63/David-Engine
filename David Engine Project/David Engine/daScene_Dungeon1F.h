#pragma once
#include "daScene.h"

namespace da
{
	class Scene_Dungeon1F : public Scene
	{
	public:
		Scene_Dungeon1F();
		virtual ~Scene_Dungeon1F();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		void addBackgroundObjects();
		void addUIObjects();

	private:
	};
}