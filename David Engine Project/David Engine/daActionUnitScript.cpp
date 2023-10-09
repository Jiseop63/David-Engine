#include "daActionUnitScript.h"
#include "daTime.h"
#include "daGameObject.h"
#include "daCreatureScript.h"
#include "daSkellBossScript.h"
#include "daSceneManager.h"
#include "daResources.h"
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
		if (enums::eUnitLifeType::Duration == mUnitTypes.LifeCycle)
		{
			mUnitInfo.DurationTime.Start += (float)Time::DeltaTime();
			if (mUnitInfo.DurationTime.End <= mUnitInfo.DurationTime.Start)
				ClearUnit();
		}
		else if (enums::eUnitLifeType::Range == mUnitTypes.LifeCycle)
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
		switch (mUnitTypes.Action)
		{
		case da::enums::eUnitActionType::Stay:
			break;
		case da::enums::eUnitActionType::UsingDirection:
		{
			math::Vector3 retPosition = mActionUnitTransform->GetPosition();
			retPosition += mUnitDirection * mUnitInfo.Speed * (float)Time::DeltaTime();
			mActionUnitTransform->SetPosition(retPosition);
		}
			break;
		case da::enums::eUnitActionType::UsingRotation:
		{
			math::Vector3 retPosition = mActionUnitTransform->GetPosition();
			retPosition += mActionUnitTransform->Up() * mUnitInfo.Speed * (float)Time::DeltaTime();
			mActionUnitTransform->SetPosition(retPosition);
		}
			break;
		default:
			break;
		}
	}
	void ActionUnitScript::OnActive()
	{
		// Transform 세팅		
		mActionUnitTransform->SetScale(mUnitScale);		

		mUnitBeginPosition = mOwnerScript->GetCreatureTransform()->GetPosition() + mUnitOffset;
		mActionUnitTransform->SetPosition(mUnitBeginPosition);
		

		// 단순 이팩트 재생
		if (enums::eUnitUsageType::JustAnimation == mUnitTypes.Usage
			|| enums::eUnitUsageType::JustTexture == mUnitTypes.Usage)
		{
			mActionUnitCollider->ApplyComponentUsing(false);
			if (enums::eUnitUsageType::JustTexture== mUnitTypes.Usage)
			{
				mActionUnitRenderer->SetMaterial(Resources::Find<Material>(L"TextureProjectileMaterial"));
				mActionUnitRenderer->ChangeMaterialTexture(Resources::Find<Texture>(mUnitName));
			}
			else if (enums::eUnitUsageType::JustAnimation == mUnitTypes.Usage)
			{
				mActionUnitRenderer->SetMaterial(Resources::Find<Material>(L"AnimationProjectileMaterial"));
				// 애니메이션만 나오는경우 3가지 유형에 따라 애니메이션 세팅
				if (enums::eUnitLifeType::AnimationEnd == mUnitTypes.LifeCycle)
					mActionUnitAnimator->PlayAnimation(mUnitInfo.Animation.Action, false);
				else 
					mActionUnitAnimator->PlayAnimation(mUnitInfo.Animation.Idle, true);			
			}
		}
		else
		{
			mActionUnitCollider->ApplyComponentUsing(true);
			mActionUnitCollider->SetSize(mUnitColliderSize);

			if (enums::eUnitUsageType::TextureProjectile == mUnitTypes.Usage)
			{
				// 텍스쳐만 나오는 경우 텍스쳐 세팅
				mActionUnitRenderer->SetMaterial(Resources::Find<Material>(L"TextureProjectileMaterial"));
				mActionUnitRenderer->ChangeMaterialTexture(Resources::Find<Texture>(mUnitName));				
			}
			else if (enums::eUnitUsageType::AnimationProjectile == mUnitTypes.Usage)
			{
				mActionUnitRenderer->SetMaterial(Resources::Find<Material>(L"AnimationProjectileMaterial"));

				if (enums::eUnitLifeType::AnimationEnd == mUnitTypes.LifeCycle)
					mActionUnitAnimator->PlayAnimation(mUnitInfo.Animation.Action, false);
				else
					mActionUnitAnimator->PlayAnimation(mUnitInfo.Animation.Idle, true);
			}
			else
			{
				mActionUnitRenderer->SetMaterial(Resources::Find<Material>(L"NoneProjectileMaterial"));
				mActionUnitRenderer->GetMaterial()->SetTexture(nullptr);
			}
		}
				
		if (enums::eUnitActionType::UsingRotation == mUnitTypes.Action)
			mActionUnitTransform->SetRotation(math::Vector3(0.0f, 0.0f, mUnitRotateAngle));
		else
			mActionUnitTransform->SetRotation(math::Vector3(0.0f, 0.0f, 0.0f));

		// active object
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void ActionUnitScript::ClearUnit()
	{
		mUnitRotateAngle = 0.0f;
		mUnitScale = math::Vector3::One;
		mUnitColliderSize = math::Vector2::One;
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
			//SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.250f);	// 카메라 진동
			// 피격 호출
			creatureScript->OnDamaged(mUnitInfo.AtaackDamage);
		}
		if (enums::eLayerType::Boss == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			SkellBossScript* bossScript = creatureObj->GetComponent<SkellBossScript>();
			// 보스 피격 호출
			mOwnerScript->CallHitEffect(bossScript->GetCreatureTransform()->GetPosition());
			bossScript->IncreaseDamageCount();
			bossScript->OnDamaged(mUnitInfo.AtaackDamage);
			//SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.250f);	// 카메라 진동
		}
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// 피격 호출
			creatureScript->OnDamaged(mUnitInfo.AtaackDamage);
		}
	}
}
