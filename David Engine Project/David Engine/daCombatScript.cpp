#include "daCombatScript.h"

#include "daTime.h"
#include "daGameObject.h"
#include "daEffectScript.h"
#include "daProjectileScript.h"
#include "daCreatureScript.h"

namespace da
{
	CombatScript::CombatScript()
		: mCombatTransform(nullptr)
		, mCombatRenderer(nullptr)
		, mCombatAnimator(nullptr)
		, mCombatEffects{}
		, mCombatProjectiles{}
		, mOwnerScript(nullptr)
		, mOwnerPosition(math::Vector3::Zero)
		, mOwnerDir(math::Vector2::UnitX)
		, mWeaponInfo(nullptr)
		, mEffectScale(math::Vector3::One)
		, mProjectileSize(math::Vector2::One)
		, mEffectAngle(0.0f)
		, mHitEffectAngle(0.0f)
		, mWeaponAttacked(false)
	{
	}
	CombatScript::~CombatScript()
	{
	}
	void CombatScript::Initialize()
	{
		mCombatTransform = GetOwner()->GetTransform();
		mCombatTransform->SetScale(math::Vector3(2.0f, 2.0f, 1.0f));

		// �ٰŸ� ����� Texture, ���Ÿ� ����� Animation �� �����
		mCombatRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mCombatAnimator = GetOwner()->AddComponent<Animator>();
	}

	EffectScript* CombatScript::callEffect()
	{
		for (size_t effect = 0; effect < mCombatEffects.size(); effect++)
		{
			if (GameObject::eObjectState::Inactive ==
				mCombatEffects[effect]->GetOwner()->GetObjectState())
				return mCombatEffects[effect];
		}
		return nullptr;
	}
	ProjectileScript* CombatScript::callProjectile()
	{
		for (size_t projectile = 0; projectile < mCombatProjectiles.size(); ++projectile)
		{
			if (GameObject::eObjectState::Inactive ==
				mCombatProjectiles[projectile]->GetOwner()->GetObjectState())
				return mCombatProjectiles[projectile];
		}
		return nullptr;
	}

	//void CombatScript::ToDoAttack()
	//{
	//	// Weapon ������ �ʿ���
	//	if (mOwnerWeapon->AttackStat.AttackReady)
	//	{
	//		attackEffect();
	//		attackProjectile();
	//		attackPlay();
	//		mOwnerWeapon->AttackStat.AttackReady = false;
	//	}
	//}

	void CombatScript::updateReverseRenderer()
	{		
		bool value = isLeft();
		mCombatRenderer->SetReverse(value);
	}

	void CombatScript::updateWeaponPosition()
	{
		// �÷��̾� ��ġ ��������
		math::Vector3 ownerPosition = mOwnerScript->GetCreatureTransform()->GetPosition();
		// �� ��ġ�� �����ϱ�
		mCombatTransform->SetPosition(ownerPosition);
		mOwnerDir = mOwnerScript->GetCreatureDir();
	}
	void CombatScript::updateAttackCoolDown()
	{
		if (!mWeaponInfo->AttackStat.AttackReady)
		{
			mWeaponInfo->AttackStat.AttackDelayAccumulateTime += (float)Time::DeltaTime();

			if (mWeaponInfo->AttackStat.AttackDelayTime <= mWeaponInfo->AttackStat.AttackDelayAccumulateTime)
			{
				mWeaponInfo->AttackStat.AttackReady = true;
				mWeaponInfo->AttackStat.AttackDelayAccumulateTime = 0.0f;
			}
		}
	}
	void CombatScript::attackEffect()
	{
		// ��ȿ�� ��ü ��������
		EffectScript* effect = callEffect();
		// ���� ���ֱ�
		math::Vector3 ownerDir(mOwnerDir.x, mOwnerDir.y, 0.0f);
		effect->SetEffectScale(mEffectScale);
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		effect->SetEffectPosition(mCombatTransform->GetPosition() + (ownerDir * mWeaponInfo->ProjectileStat.ProjectileCenterPadding));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(mWeaponInfo->ProjectileStat.EffectName);
	}
	void CombatScript::attackPlay()
	{
		// combat Ŭ���������� �ϴ� �̷��� �����, ��ӹ��� ��ü�� weapon������ ����ɰ�

		if (mWeaponInfo->IsAnimationType)
		{
			// Play animation 
			mCombatAnimator->PlayAnimation(mWeaponInfo->AnimationName, false);	// �ִϸ��̼� ȣ��
		}
		else
		{
			// Change texture
			if (mWeaponAttacked)
				mWeaponAttacked = false;
			else
				mWeaponAttacked = true;			
		}
	}

	void CombatScript::CallHitEffect(math::Vector3 position)
	{
		if (7 <= mHitEffectAngle)
			mHitEffectAngle = 0.0f;
		mHitEffectAngle += 1.80f;
		// ���� ���ϱ�
		EffectScript* effect = callEffect();
		effect->SetEffectScale(math::Vector3(1.50f, 1.50f, 1.0f));
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mHitEffectAngle));
		effect->SetEffectPosition(position - math::Vector3(0.0f, 0.2f, 0.0f));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(L"Slash");
	}
}