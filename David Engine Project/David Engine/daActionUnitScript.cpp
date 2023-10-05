#include "daActionUnitScript.h"
#include "daTime.h"
#include "daGameObject.h"
#include "daCreatureScript.h"
#include "daSkellBossScript.h"
#include "daSceneManager.h"
namespace da
{
	ActionUnitScript::ActionUnitScript()
		: mActionUnitTransform(nullptr)
		, mActionUnitCollider(nullptr)
		, mActionUnitRenderer(nullptr)
		, mActionUnitAnimator(nullptr)
		, mOwnerScript(nullptr)
		, mUnitBeginPosition(math::Vector3::Zero)
		, mUnitDirection(math::Vector3::UnitY)
		, mUnitRotateAngle(0.0f)
		, mUnitInfo{}
		, mUnitTypes{}
		, mUnitAnimationInfo{}
		, mUnitAttackStat{}
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
		if (enums::eUnitActionType::Duration == mUnitTypes.ActionType)
		{
			mUnitInfo.DurationTime.Start += (float)Time::DeltaTime();
			if (mUnitInfo.DurationTime.End <= mUnitInfo.DurationTime.Start)
				ClearUnit();
		}
		else if (enums::eUnitActionType::Range == mUnitTypes.ActionType)
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
		switch (mUnitTypes.RenderType)
		{
		case da::enums::eUnitRenderType::Stay:
			break;
		case da::enums::eUnitRenderType::UsingDirection:
		{
			math::Vector3 retPosition = mActionUnitTransform->GetPosition();
			retPosition += mUnitDirection * mUnitInfo.Speed * (float)Time::DeltaTime();
			mActionUnitTransform->SetPosition(retPosition);
		}
			break;
		case da::enums::eUnitRenderType::UsingRotation:
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
		mActionUnitTransform->SetScale(mUnitScale);
		mUnitBeginPosition = mOwnerScript->GetCreatureTransform()->GetPosition() + mUnitOffset;
		if (enums::eUnitRenderType::JustRotate == mUnitTypes.RenderType
			|| enums::eUnitRenderType::UsingRotation == mUnitTypes.RenderType)
		{
			mActionUnitTransform->SetRotation(math::Vector3(0.0f, 0.0f, mUnitRotateAngle));
		}
		else
			mActionUnitTransform->SetRotation(math::Vector3(0.0f, 0.0f, 0.0f));
		mActionUnitTransform->SetPosition(mUnitBeginPosition);

		// collider 세팅
		if (!(enums::eUnitUsageType::OnlyTexture == mUnitTypes.UsageType
			|| enums::eUnitUsageType::OnlyAnimation == mUnitTypes.UsageType))
		{
			mActionUnitCollider->ApplyComponentUsing(true);
		}
		else
		{
			mActionUnitCollider->ApplyComponentUsing(false);
		}

		// animation 활성화
		if (enums::eUnitUsageType::Default == mUnitTypes.UsageType
			|| enums::eUnitUsageType::OnlyAnimation == mUnitTypes.UsageType)
		{
			mActionUnitAnimator->ApplyComponentUsing(true);
			mActionUnitAnimator->PlayAnimation(mUnitAnimationInfo.Name, mUnitAnimationInfo.Loop);
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
			SceneManager::GetMainCameraScript()->SetOscillation(30.0f, 0.1250f);
			
			// 피격 호출
			creatureScript->OnDamaged(mUnitAttackStat.AtaackDamage);
		}
		if (enums::eLayerType::Boss == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			SkellBossScript* bossScript = creatureObj->GetComponent<SkellBossScript>();
			// 보스 피격 호출
			mOwnerScript->CallHitEffect(bossScript->GetCreatureTransform()->GetPosition());
			bossScript->IncreaseDamageCount();
			SceneManager::GetMainCameraScript()->SetOscillation(30.0f, 0.1250f);	// 카메라 진동
		}
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// 피격 호출
			creatureScript->OnDamaged(mUnitAttackStat.AtaackDamage);
		}
	}
}
