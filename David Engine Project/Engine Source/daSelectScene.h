#pragma once
#include "daScene.h"

namespace da
{
	class SelectScene : public Scene
	{
	public:
		SelectScene();
		virtual ~SelectScene();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	};
}