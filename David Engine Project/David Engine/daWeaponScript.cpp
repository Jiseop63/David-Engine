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
		// �� Ÿ�Կ� ���� ������ �޶���

		// ������ ��� �浹ü::���� �������ְ�,
		// �̹��� ���� + ����Ʈ ���

		PlayAnimation();
		AttackLogic();


	}
	void WeaponScript::PlayAnimation()
	{
		// ������ ��� �̹��� ����
		ChangeTexture();
		
		// ���Ÿ��� ��� �ִϸ��̼� ���
		// to do...
		
		// ����Ʈ ���
		// to do...

	}
	void WeaponScript::ChangeTexture()
	{
	}
	void WeaponScript::AttackLogic()
	{
		// ������ ��� �浹ü �������ֱ�
		// to do...

		// ���Ÿ��� ��� ����ü �����ֱ�
		// to do...

	}
}