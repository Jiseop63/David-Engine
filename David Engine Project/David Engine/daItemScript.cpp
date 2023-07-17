#include "daItemScript.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
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
	void ItemScript::SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		meshRenderer->ChangeTexture(first);
	}
	void ItemScript::SetScreenPosision()
	{
		// �� ũ�� ���ϱ�
		Transform* myTransform = GetOwner()->GetComponent<Transform>();
		Vector3 myScale = myTransform->GetScale();
		mSize = Vector2(myScale.x * 100, myScale.y * 100);

		// �� ��ġ ���ϱ�
		Vector3 myPos = myTransform->GetPosition();
		Vector3 myScreenPos = myTransform->GetScreenPosition();
		mScreenPosition = Vector2(myScreenPos.x, myScreenPos.y);
	}
	void ItemScript::ChangeTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeTexture(texture);
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

		// �浹��
		if (mousePosition.x >= left
			&& mousePosition.x <= right
			&& mousePosition.y >= top
			&& mousePosition.y <= bottom)
		{
			ChangeTexture(mSecondTexture);
			mFocusOn = true;
		}
		else if (mFocusOn = true)
		{
			ChangeTexture(mFirstTexture);
			mFocusOn = false;
		}
	}
}