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
		// ���� * cos (�̵��� * ��ȯ�ӵ�) / �̵��ӵ� * DT
		position += mTransform->Up() * cos(mMoveValue * mTurnSpeed) / mMoveSpeed * (float)Time::DeltaTime();

		mTransform->SetPosition(position);
	}
}