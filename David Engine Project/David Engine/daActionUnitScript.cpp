#include "daActionUnitScript.h"
#include "daGameObject.h"
#include "daCreatureScript.h"
#include "daTime.h"
namespace da
{
	ActionUnitScript::ActionUnitScript()
		: mActionUnitTransform(nullptr)
		, mActionUnitCollider(nullptr)
		, mActionUnitRenderer(nullptr)
		, mActionUnitAnimator(nullptr)
		, mUnitInfo{}
		, mUnitActionType(UnitActionType::Stay)
		, mUnitUsageType(UnitUsageType::Default)
		, mUnitBeginPosition(math::Vector3::Zero)
		, mUnitDirection(math::Vector3::UnitY)
		, mUnitAnimationName()
		, mUnitAnimationLoop(false)
	{
	}
	ActionUnitScript::~ActionUnitScript()
	{
	}
	void ActionUnitScript::Initialize()
	{
		mActionUnitTransform = GetOwner()->GetTransform();
		mActionUnitCollider = GetOwner()->AddComponent<Collider2D>();
		mActionUnitRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mActionUnitAnimator = GetOwner()->AddComponent<Animator>();
	}
	void ActionUnitScript::Update()
	{
		// 종료 조건
		if (UnitActionType::Stay == mUnitActionType
			|| UnitActionType::JustRotate == mUnitActionType)
		{
			mUnitInfo.Time.Start += (float)Time::DeltaTime();
			if (mUnitInfo.Time.End <= mUnitInfo.Time.Start)
				ClearUnit();
		}
		else
		{
			math::Vector3 currentPosition = mActionUnitTransform->GetPosition();
			math::Vector3 moveDistance = mUnitBeginPosition - currentPosition;
			if (mUnitInfo.Range <= moveDistance.Length())
				ClearUnit();
		}
	}
	void ActionUnitScript::LateUpdate()
	{
		// 이동 조건
		switch (mUnitActionType)
		{
		case da::UnitActionType::Stay:
			break;
		case da::UnitActionType::UsingDirection:
		{
			math::Vector3 retPosition = mActionUnitTransform->GetPosition();
			retPosition += mUnitDirection * mUnitInfo.Speed * (float)Time::DeltaTime();
			mActionUnitTransform->SetPosition(retPosition);
		}
			break;
		case da::UnitActionType::UsingRotation:
		{
			math::Vector3 retPosition = mActionUnitTransform->GetPosition();
			retPosition += mActionUnitTransform->Up() * mUnitInfo.Speed * (float)Time::DeltaTime();
			mActionUnitTransform->SetPosition(retPosition);
		}
			break;
		default:
			break;
		}
		// 비주얼 조건
		// 따로 뭐 없을듯?
	}
	void ActionUnitScript::OnActive()
	{
		mUnitBeginPosition = mOwnerScript->GetCreatureTransform()->GetPosition() + mOffsetPosition;
		// Transform 세팅		
		if (UnitActionType::JustRotate == mUnitActionType
			|| UnitActionType::UsingRotation == mUnitActionType)
		{
			mActionUnitTransform->SetRotation(math::Vector3(0.0f, 0.0f, mUnitInfo.RotateAngle));
			/*math::Vector3 dir = mOffsetPosition.y * mActionUnitTransform->Up();
			mUnitBeginPosition = mOwnerScript->GetCreatureTransform()->GetPosition() + dir;*/
		}
		mActionUnitTransform->SetPosition(mUnitBeginPosition);


		// collider 세팅
		if (UnitUsageType::Default == mUnitUsageType
			|| UnitUsageType::OnlyCollider == mUnitUsageType)
		{
			mActionUnitCollider->ApplyComponentUsing(true);
			// collider detail
		}
		else
		{
			mActionUnitCollider->ApplyComponentUsing(false);
		}

		// animation 활성화
		if (UnitUsageType::Default == mUnitUsageType
			|| UnitUsageType::OnlyAnimation == mUnitUsageType)
		{
			mActionUnitAnimator->ApplyComponentUsing(true);
			mActionUnitAnimator->PlayAnimation(mUnitAnimationName, mUnitAnimationLoop);
		}
		else
		{
			mActionUnitAnimator->ApplyComponentUsing(false);
		}
		// active object
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void ActionUnitScript::ClearUnit()
	{
		// 각종 변수 초기화 해주기
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}

	void ActionUnitScript::OnCollisionEnter(Collider2D* other)
	{

	}
}
