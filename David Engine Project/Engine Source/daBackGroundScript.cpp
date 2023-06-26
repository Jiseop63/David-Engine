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
			// 움직임 반복 로직
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
		// 나중에 cos 계산을 셰이더 코드에서 진행하도록 해보자

		Vector3 position = mTransform->GetPosition();
		mMoveValue += (float)Time::DeltaTime();
		// 방향 * 이동값 ()  * 시간
		position += mTransform->Up() * cos(mMoveValue) / mMoveSpeed * (float)Time::DeltaTime();

		mTransform->SetPosition(position);
	}
}