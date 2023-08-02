#pragma once
#include "daScene.h"

namespace da
{
	class Scene_Title : public Scene
	{
	public:
		Scene_Title();
		virtual ~Scene_Title();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		
		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:
		void addBackgroundObjects();
		void addUIObjects();
		void initializeCommonObjects();

	private:
	};
}