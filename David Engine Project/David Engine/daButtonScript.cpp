#include "daButtonScript.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daInput.h"

#include "daUIObject.h"

namespace da
{
	using namespace math;
	ButtonScript::ButtonScript()
		: mFocusOn(false)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
		, mScreenPosition(Vector2::Zero)
		, mSize(Vector2::Zero)
	{
	}
	ButtonScript::~ButtonScript()
	{
	}

	void ButtonScript::Update()
	{
		focusCheck();
		if (mFocusOn)
		{
			if (Input::GetKeyDown(eKeyCode::LBTN)
				|| Input::GetKeyDown(eKeyCode::F))
			{
				dynamic_cast<UIObject*>(GetOwner())->ExcuteEvent();
			}
		}		
	}

	void ButtonScript::SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		meshRenderer->ChangeTexture(first);
	}

	void ButtonScript::SetScreenPosision()
	{
		Transform* myTransform = GetOwner()->GetComponent<Transform>();
		// 내 크기 구하기
		Vector3 myScale = myTransform->GetScale();
		mSize = Vector2(myScale.x * 100, myScale.y * 100);

		// 내 위치 구하기
		Vector3 myScreenPos = myTransform->GetScreenPosition();		 
		mScreenPosition = Vector2( myScreenPos.x, myScreenPos.y );
	}

	void ButtonScript::focusCheck()
	{
		Vector2 mousePosition = Input::GetMouseScreenPosition();	

		float left		= mScreenPosition.x - (mSize.x / 2);
		float top		= mScreenPosition.y + (mSize.y / 2);
		Vector2 LT(left, top);

		float right		= mScreenPosition.x + (mSize.x / 2);
		float bottom	= mScreenPosition.y - (mSize.y / 2);
		Vector2 RB(right, bottom);

		// 충돌중
		if (mousePosition.x >= left
			&& mousePosition.x <= right
			&& mousePosition.y <= top
			&& mousePosition.y >= bottom)
		{
			changeTexture(mSecondTexture);
			mFocusOn = true;
		}
		else if (mFocusOn = true)
		{
			changeTexture(mFirstTexture);
			mFocusOn = false;
		}
	}
	void ButtonScript::changeTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeTexture(texture);
	}
}