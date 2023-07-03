#include "daButtonScript.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"

#include "daInput.h"

namespace da
{
	using namespace math;
	ButtonScript::ButtonScript()
		: mIsChanged(false)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
		, mScreenPosition(Vector2::Zero)
		, mSize(Vector2::Zero)
	{
	}
	ButtonScript::~ButtonScript()
	{
	}
	void ButtonScript::Initialize()
	{
	}
	void ButtonScript::Update()
	{
		if (Input::GetKeyDown(eKeyCode::A))
		{
			int a = 0;
		}
		isCollisionMouse();
	}
	void ButtonScript::LateUpdate()
	{
	}
	void ButtonScript::Render()
	{
	}
	void ButtonScript::SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		meshRenderer->ChangeTexture(first);
	}

	void ButtonScript::changeTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeTexture(texture);
	}

	void ButtonScript::SetScreenPosision()
	{
		Transform* myTransform = GetOwner()->GetComponent<Transform>();
		Vector3 myScreenPos = myTransform->GetScreenPosition();
		Vector3 myScale = myTransform->GetScale();
		
		mSize = Vector2(myScale.x * 100, myScale.y * 100);
		mScreenPosition = Vector2(myScreenPos.x, myScreenPos.y);
	}
	void ButtonScript::isCollisionMouse()
	{
		
		
		Vector2 mousePosition = Input::GetMouseScreenPos();	

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
			mIsChanged = true;
		}
		else if (mIsChanged = true)
		{
			changeTexture(mFirstTexture);
			mIsChanged = false;
		}
		// 충돌이 아님
		
	}
}