#include "daEnemyWeaponScript.h"
#include "daGameObject.h"

namespace da
{
	EnemyWeaponScript::EnemyWeaponScript()
		: mEnemyWeaponTransform(nullptr)
		, mEnemyWeaponRenderer(nullptr)
		, mEnemyWeaponAnimator(nullptr)
	{
	}
	EnemyWeaponScript::~EnemyWeaponScript()
	{
	}
	void EnemyWeaponScript::Initialize()
	{
		mEnemyWeaponTransform = GetOwner()->GetComponent<Transform>();
		mEnemyWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mEnemyWeaponAnimator = GetOwner()->AddComponent<Animator>();

	}
	void EnemyWeaponScript::Update()
	{
	}
	void EnemyWeaponScript::SetWeaponTransform(math::Vector3 ownerPos, math::Vector2 ownerDir)
	{
		// Player ��ġ ����
		ownerPos;

		// Player ���� ���ϱ�
		ownerDir;
		
		bool isLeft = false;
		if (0 >= ownerDir.x)
			isLeft = true;

		mEnemyWeaponTransform->SetPosition(ownerPos);
	}
	void EnemyWeaponScript::DoAttack()
	{

	}
	void EnemyWeaponScript::activeAttack()
	{

	}
}
