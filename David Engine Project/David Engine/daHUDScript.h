#pragma once
#include "daScript.h"

namespace da
{
	class Scene;
	class HUDScript : public Script
	{
	public:
		HUDScript();
		virtual ~HUDScript();

		// UI 추가하는 기능
		void PushBackUIObject(GameObject* object) { mUIs.push_back(object); }
		// UI 등록하는 기능
		void RegisteUI(Scene* scene);
		// UI 제어하는 기능
		std::vector<GameObject*> GetUIs() { return mUIs; }

	private:
		std::vector<GameObject*> mUIs;
	};
}