#include "daHUDScript.h"
#include "daScene.h"
namespace da
{
	HUDScript::HUDScript()
		: mUIs{}
	{
	}
	HUDScript::~HUDScript()
	{
	}
	void HUDScript::RegisteUI(Scene* scene)
	{
		for (GameObject* object : mUIs)
		{
			scene->AddGameObject(enums::eLayerType::UI, object);
		}
	}
}

