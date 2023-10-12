#include "daCombatScript.h"

#include "daTime.h"
#include "daGameObject.h"
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
		, mWeaponAngle(0.0f)
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

		// 근거리 무기는 Texture, 원거리 무기는 Animation 을 사용함
		mCombatRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mCombatAnimator = GetOwner()->AddComponent<Animator>();
	}

	void CombatScript::updateReverseRenderer()
	{
		mOwnerDir = mOwnerScript->GetCreatureDir();
		bool value = isLeft();
		/*if (value)*/
		
		mCombatRenderer->SetReverse(value);
	}

	void CombatScript::updateWeaponPosition()
	{
		// 플레이어 위치 가져오기
		mOwnerPosition = mOwnerScript->GetCreatureTransform()->GetPosition();
		// 내 위치에 적용하기
		mCombatTransform->SetPosition(mOwnerPosition);
	}
}