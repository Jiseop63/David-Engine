#pragma once
#include "daScene.h"

namespace da
{
	class TitleScene : public Scene
	{
	public:
		TitleScene();
		virtual ~TitleScene();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		
		virtual void AddBackgroundObject() override;

	private:
	};
}