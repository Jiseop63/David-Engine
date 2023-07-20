#pragma once
#include "daScene.h"

namespace da
{
	class Scene_Dungeon1F : public Scene
	{
	public:
		enum class Stage
		{
			Stage1,
			Stage2,
			EntryBossRoom,
		};
		Scene_Dungeon1F();
		virtual ~Scene_Dungeon1F();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	public:
		void State1();
		void Stage2();
		void EntryBossRoom();


	private:
		void addBackgroundObjects();
		void addUIObjects();
		void addGameObjects();
	private:
	};
}