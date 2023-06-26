#pragma once
#include "daScene.h"

namespace da
{
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		virtual ~PlayScene();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void InitBGObj();

	};
}