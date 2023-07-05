#pragma once
#include "daScene.h"

namespace da
{
	class Scene_Town : public Scene
	{
	public:
		Scene_Town();
		virtual ~Scene_Town();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		void addBackgroundObject();
		void addUIObjects();

	private:
	};
}