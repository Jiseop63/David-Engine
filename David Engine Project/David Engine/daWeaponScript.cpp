#include "daWeaponScript.h"

namespace da
{
	WeaponScript::WeaponScript()
		: mEffectObject(nullptr)
		, mAnimator(nullptr)
		, mRenderer(nullptr)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
		, mWeaponAttacked(false)
	{
	}
	WeaponScript::~WeaponScript()
	{
	}
	void WeaponScript::Update()
	{
	}
	void WeaponScript::WeaponAttack()
	{
		// 내 타입에 따라서 로직이 달라짐

		// 근접의 경우 충돌체::공격 세팅해주고,
		// 이미지 변경 + 이펙트 재생

		PlayAnimation();
		AttackLogic();


	}
	void WeaponScript::PlayAnimation()
	{
		// 근접인 경우 이미지 변경
		ChangeTexture();
		
		// 원거리인 경우 애니메이션 재생
		// to do...
		
		// 이팩트 재생
		// to do...

	}
	void WeaponScript::ChangeTexture()
	{
	}
	void WeaponScript::AttackLogic()
	{
		// 근접인 경우 충돌체 세팅해주기
		// to do...

		// 원거리인 경우 투사체 날려주기
		// to do...

	}
}