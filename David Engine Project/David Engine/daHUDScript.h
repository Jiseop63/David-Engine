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

		// UI �߰��ϴ� ���
		void PushBackUIObject(GameObject* object) { mUIs.push_back(object); }
		// UI ����ϴ� ���
		void RegisteUI(Scene* scene);
		// UI �����ϴ� ���
		std::vector<GameObject*> GetUIs() { return mUIs; }

	private:
		std::vector<GameObject*> mUIs;
	};
}