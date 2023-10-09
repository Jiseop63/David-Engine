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
		// ���� ����
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
		// �̵� ����
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
		// Transform ����		
		mActionUnitTransform->SetScale(mUnitScale);		

		mUnitBeginPosition = mOwnerScript->GetCreatureTransform()->GetPosition() + mUnitOffset;
		mActionUnitTransform->SetPosition(mUnitBeginPosition);
		

		// �ܼ� ����Ʈ ���
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
				// �ִϸ��̼Ǹ� �����°�� 3���� ������ ���� �ִϸ��̼� ����
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
				// �ؽ��ĸ� ������ ��� �ؽ��� ����
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
		// ���� ���� �ʱ�ȭ ���ֱ�
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}

	void ActionUnitScript::OnCollisionEnter(Collider2D* other)
	{
		
		if (enums::eLayerType::Monster == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			// �ǰ� ȣ��
			mOwnerScript->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());			
			//SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.250f);	// ī�޶� ����
			// �ǰ� ȣ��
			creatureScript->OnDamaged(mUnitInfo.AtaackDamage);
		}
		if (enums::eLayerType::Boss == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			SkellBossScript* bossScript = creatureObj->GetComponent<SkellBossScript>();
			// ���� �ǰ� ȣ��
			mOwnerScript->CallHitEffect(bossScript->GetCreatureTransform()->GetPosition());
			bossScript->IncreaseDamageCount();
			bossScript->OnDamaged(mUnitInfo.AtaackDamage);
			//SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.250f);	// ī�޶� ����
		}
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// �ǰ� ȣ��
			creatureScript->OnDamaged(mUnitInfo.AtaackDamage);
		}
	}
}
