#include "daCombatScript.h"

#include "daTime.h"
#include "daGameObject.h"
#include "daEffectScript.h"
#include "daCreatureScript.h"
#include "daActionUnitScript.h"

namespace da
{
	CombatScript::CombatScript()
		: mCombatTransform(nullptr)
		, mCombatRenderer(nullptr)
		, mCombatAnimator(nullptr)
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
}