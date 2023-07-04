#include "daUIObject.h"

#include "daButtonScript.h"
#include "daSceneManager.h"
namespace da
{
	UIObject::UIObject()
	{
	}
	UIObject::~UIObject()
	{
	}
	void UIObject::Initialize()
	{
		GameObject::Initialize();
	}
	void UIObject::Update()
	{
		GameObject::Update();
	}
	void UIObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void UIObject::Render()
	{
		GameObject::Render();
	}
	void UIObject::ExcuteEvent()
	{
		int a = 0;
		ButtonScript* uiScript = GetComponent<ButtonScript>();
		if (uiScript)
		{
			if (ButtonScript::eButtonType::Play == uiScript->GetButtonType())
			{
				// Town¾ÀÀ¸·Î ÀÌµ¿
				SceneManager::LoadScene(L"Scene_Town");
			}
			if (ButtonScript::eButtonType::Exit == uiScript->GetButtonType())
			{
				PostQuitMessage(WM_QUIT);
			}
		}
	}
}
