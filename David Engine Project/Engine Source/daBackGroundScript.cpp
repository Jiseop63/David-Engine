#include "daBackGroundScript.h"

#include "daTime.h"

#include "daGameObject.h"
#include "daTransform.h"

namespace da
{
	using namespace math;

	BackGroundScript::BackGroundScript()
		: mTransform(nullptr)
		, mIsMovingObj(true)
		, mMoveValue(0.0f)
		, mMoveSpeed(3.5f)
	{
	}
	BackGroundScript::~BackGroundScript()
	{
	}

	void BackGroundScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}
	void BackGroundScript::Update()
	{
		if (true == mIsMovingObj)
		{
			// ������ �ݺ� ����
			movePeriodically();
		}
		
	}
	void BackGroundScript::LateUpdate()
	{
	}
	void BackGroundScript::Render()
	{
	}
	void BackGroundScript::movePeriodically()
	{
		// ���߿� cos ����� ���̴� �ڵ忡�� �����ϵ��� �غ���

		Vector3 position = mTransform->GetPosition();
		mMoveValue += (float)Time::DeltaTime();
		// ���� * �̵��� ()  * �ð�
		position += mTransform->Up() * cos(mMoveValue) / mMoveSpeed * (float)Time::DeltaTime();

		mTransform->SetPosition(position);
	}
}