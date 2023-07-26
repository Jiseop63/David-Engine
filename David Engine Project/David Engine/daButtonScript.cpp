#include "daButtonScript.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daInput.h"

#include "daUIObject.h"

namespace da
{
	using namespace math;
	ButtonScript::ButtonScript()
		: mButtonType(eButtonType::Default)		
	{
	}
	ButtonScript::~ButtonScript()
	{
	}

	void ButtonScript::Update()
	{
		UIScript::focusCheck();
		if (mFocusOn)
		{
			if (Input::GetKeyDown(eKeyCode::LBTN)
				|| Input::GetKeyDown(eKeyCode::F))
			{
				dynamic_cast<UIObject*>(GetOwner())->ExcuteEvent();
			}
		}		
	}
}