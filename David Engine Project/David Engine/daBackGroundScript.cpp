#include "daBackGroundScript.h"

#include "daTime.h"

#include "daGameManager.h"

#include "daGameObject.h"
#include "daTransform.h"

namespace da
{
	using namespace math;

	BackGroundScript::BackGroundScript()
		: mTransform(nullptr)
		, mIsMovingObj(true)
		, mMoveValue(0.0f)
		, mTurnSpeed(3.5f)
		, mMoveSpeed(10.5f)
	{
	}
	BackGroundScript::~BackGroundScript()
	{
	}

	void BackGroundScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
		int addTurnSpeedValue = rand() % 5;
		int addMoveSpeedValue = rand() % 10;

		mTurnSpeed += addTurnSpeedValue;
		mMoveSpeed += addMoveSpeedValue;
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
		// 방향 * cos (이동값 * 전환속도) / 이동속도 * DT
		position += mTransform->Up() * cos(mMoveValue * mTurnSpeed) / mMoveSpeed * (float)Time::DeltaTime();

		mTransform->SetPosition(position);
	}
}