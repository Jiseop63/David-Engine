#include "daItemScript.h"

#include "daGameObject.h"

#include "daInput.h"
namespace da
{
	using namespace math;

	ItemScript::ItemScript()
		: mFocusOn(false)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
		, mScreenPosition(Vector2::Zero)
		, mSize(Vector2::Zero)
	{
	}
	ItemScript::~ItemScript()
	{
	}
	void ItemScript::Update()
	{
		focusCheck();
	}
	void ItemScript::SetSlotTextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		mSlotRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		mSlotRenderer->ChangeMaterialTexture(first);
	}
	void ItemScript::SetScreenPosision()
	{
		// 내 크기 구하기
		Transform* myTransform = GetOwner()->GetComponent<Transform>();
		Vector3 myScale = myTransform->GetScale();
		mSize = Vector2(myScale.x * 100, myScale.y * 100);

		// 내 위치 구하기
		Vector3 myPos = myTransform->GetPosition();

		if (0 < myPos.x)
			mScreenPosition.x = MaxPositionX * 100.0f + (myPos.x * 100.0f);
		else
			mScreenPosition.x = MaxPositionX * 100.0f - (myPos.x * 100.0f);

		mScreenPosition.y = MaxPositionY * 100.0f - (myPos.y * 100.0f);
	}
	void ItemScript::ChangeItemTexture(std::shared_ptr<graphics::Texture> texture)
	{
		mItemRenderer->ChangeMaterialTexture(texture);
	}
	void ItemScript::ChangeSlotTexture(std::shared_ptr<graphics::Texture> texture)
	{		
		mSlotRenderer->ChangeMaterialTexture(texture);
	}
	void ItemScript::focusCheck()
	{
		Vector2 mousePosition = Input::GetMouseScreenPosition();

		float left = mScreenPosition.x - (mSize.x / 2);
		float top = mScreenPosition.y - (mSize.y / 2);
		Vector2 LT(left, top);

		float right = mScreenPosition.x + (mSize.x / 2);
		float bottom = mScreenPosition.y + (mSize.y / 2);
		Vector2 RB(right, bottom);

		// 충돌중
		if (mousePosition.x >= left
			&& mousePosition.x <= right
			&& mousePosition.y >= top
			&& mousePosition.y <= bottom)
		{
			ChangeSlotTexture(mSecondTexture);
			mFocusOn = true;
		}
		else if (mFocusOn = true)
		{
			ChangeSlotTexture(mFirstTexture);
			mFocusOn = false;
		}
	}
}