#pragma once
#include "daScript.h"

namespace da
{
	class HUDScript : public Script
	{
	public:
		HUDScript();
		virtual ~HUDScript();

		virtual void Initialize() override;
		virtual void Update()override;
		virtual void LateUpdate()override;
		virtual void Render()override;

		void SetPanel(GameObject* panel) { mPanelUI = panel; }
		void SetValue(GameObject* value) { mValueUI = value; }

	private:
		GameObject* mPanelUI;
		GameObject* mValueUI;
	};
}