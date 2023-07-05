#pragma once
#include "daScene.h"

namespace da
{
	class Scene_Dungeon2F : public Scene
	{
	public:
		Scene_Dungeon2F();
		virtual ~Scene_Dungeon2F();
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