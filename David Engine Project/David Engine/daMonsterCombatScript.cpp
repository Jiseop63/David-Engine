#include "daMonsterCombatScript.h"

#include "daGameDataManager.h"
#include "daTime.h"
#include "daInput.h"

#include "daResources.h"
#include "daGameObject.h"

#include "daAnimator.h"
#include "daMeshRenderer.h"

#include "daMonsterScript.h"

namespace da
{
	MonsterCombatScript::MonsterCombatScript()
		: mEnemyWeaponTransform(nullptr)
		, mEnemyWeaponRenderer(nullptr)
		, mEnemyWeaponAnimator(nullptr)
		, mEnemyWeaponCollider(nullptr)
		, mEnemyWeaponTexture(nullptr)
		, mMonsterScript(nullptr)
		, mPlayerDamaged(false)
	{
	}
	MonsterCombatScript::~MonsterCombatScript()
	{
	}
	void MonsterCombatScript::Initialize()
	{
		mEnemyWeaponTransform = GetOwner()->GetComponent<Transform>();
		mEnemyWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mEnemyWeaponAnimator = GetOwner()->AddComponent<Animator>();
		mEnemyWeaponCollider = GetOwner()->AddComponent<Collider2D>();
		
		mEnemyWeaponTransform->SetScale(math::Vector3(2.50f, 2.50f, 1.0f));
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);

		// �ִϸ��̼� & �̺�Ʈ
		
		mEnemyWeaponAnimator->Create(L"EnemyGreatSwordIdle", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 1, math::Vector2::Zero, 0.0f, 70.0f);
		
		
		mEnemyWeaponAnimator->Create(L"EnemyGreatSwordSwing", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 16, math::Vector2::Zero, 0.050f, 70.0f);
		mEnemyWeaponAnimator->ActionEvent(L"EnemyGreatSwordSwing", 7) = std::bind(&MonsterCombatScript::activeAttack, this);
		mEnemyWeaponAnimator->CompleteEvent(L"EnemyGreatSwordSwing") = std::bind(&MonsterCombatScript::inactiveAttack, this);
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordIdle", false);
	}
	void MonsterCombatScript::Update()
	{
	}
	void MonsterCombatScript::LateUpdate()
	{
		mEnemyWeaponTransform->SetPosition(mMonsterScript->GetCreatureTransform()->GetPosition());
		updateReverseRenderer();
	}
	void MonsterCombatScript::DoAttack()
	{
		// �ִϸ��̼�
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordSwing", false);
		// �浹ü Ȱ��ȭ	*�׼��̺�Ʈ���� �����ؾ���
		// mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}

	// Ư�� �����ӿ� ȣ���
	void MonsterCombatScript::activeAttack()
	{
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}

	void MonsterCombatScript::inactiveAttack()
	{
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);
	}

	bool MonsterCombatScript::isLeft()
	{
		if (0 >= mMonsterScript->GetCreatureDir().x) 
			return true; 
		else 
			return false;
	}

	void MonsterCombatScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			CreatureScript* playerScript = other->GetOwner()->GetComponent<CreatureScript>();
			playerScript->OnDamaged(4.0f);
			mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);				
		}		
	}
}
