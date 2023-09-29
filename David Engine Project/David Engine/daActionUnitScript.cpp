#include "daActionUnitScript.h"
#include "daTime.h"
#include "daGameObject.h"
#include "daCreatureScript.h"
#include "daSkellBossScript.h"
namespace da
{
	ActionUnitScript::ActionUnitScript()
		: mActionUnitTransform(nullptr)
		, mActionUnitCollider(nullptr)
		, mActionUnitRenderer(nullptr)
		, mActionUnitAnimator(nullptr)
		, mOwnerScript(nullptr)
		, mUnitInfo{}
		, mUnitUsageType(UnitUsageType::Default)
		, mUnitActionType(UnitActionType::None)
		, mUnitRenderType(UnitRenderType::Stay)
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
		if (UnitActionType::Duration == mUnitActionType)
		{
			mUnitInfo.Time.Start += (float)Time::DeltaTime();
			if (mUnitInfo.Time.End <= mUnitInfo.Time.Start)
				ClearUnit();
		}
		else if (UnitActionType::Range == mUnitActionType)
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
		switch (mUnitRenderType)
		{
		case da::UnitRenderType::Stay:
			break;
		case da::UnitRenderType::UsingDirection:
		{
			math::Vector3 retPosition = mActionUnitTransform->GetPosition();
			retPosition += mUnitDirection * mUnitInfo.Speed * (float)Time::DeltaTime();
			mActionUnitTransform->SetPosition(retPosition);
		}
			break;
		case da::UnitRenderType::UsingRotation:
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
		// Transform 세팅		
		mActionUnitTransform->SetScale(math::Vector3(mUnitInfo.Scale, mUnitInfo.Scale, 1.0f));
		mUnitBeginPosition = mOwnerScript->GetCreatureTransform()->GetPosition() + mOffsetPosition;
		if (UnitRenderType::JustRotate == mUnitRenderType
			|| UnitRenderType::UsingRotation == mUnitRenderType)
		{
			mActionUnitTransform->SetRotation(math::Vector3(0.0f, 0.0f, mUnitInfo.RotateAngle));
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
		
		if (enums::eLayerType::Monster == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			// 피격 호출
			mOwnerScript->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());			
			//SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);

			// 최종 피해량
			float totalDamage = 6.0f;


			// 피격 호출
			creatureScript->OnDamaged(totalDamage);
		}
		if (enums::eLayerType::Boss == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			SkellBossScript* bossScript = creatureObj->GetComponent<SkellBossScript>();
			// 보스 피격 호출
			mOwnerScript->CallHitEffect(bossScript->GetCreatureTransform()->GetPosition());
			bossScript->IncreaseDamageCount();
			//SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);	// 카메라 진동
		}
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// 최종 피해량
			float totalDamage = 5.0f;
			// 피격 호출
			creatureScript->OnDamaged(totalDamage);
		}
	}
}
