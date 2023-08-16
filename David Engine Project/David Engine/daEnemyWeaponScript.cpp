#include "daEnemyWeaponScript.h"
#include "daGameDataManager.h"
#include "daTime.h"

#include "daResources.h"
#include "daGameObject.h"

#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	EnemyWeaponScript::EnemyWeaponScript()
		: mEnemyWeaponTransform(nullptr)
		, mEnemyWeaponRenderer(nullptr)
		, mEnemyWeaponAnimator(nullptr)
		, mEnemyWeaponCollider(nullptr)
		, mEnemyWeaponTexture(nullptr)
		, mCreatureAttackReady(false)
		, mCreatureAttackAccumulateTime(0.0f)
		, mCreatureAttackDelayTime(0.0f)
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
		mEnemyWeaponCollider = GetOwner()->AddComponent<Collider2D>();

		mCreatureAttackDelayTime = 3.20f;

		

		mEnemyWeaponAnimator->Create(L"EnemyGreatSword", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 16, math::Vector2::Zero, 0.050f, 45.0f);
	}
	void EnemyWeaponScript::Update()
	{
		// ���� ������ ����

		if (!mCreatureAttackReady)
		{
			mCreatureAttackAccumulateTime += (float)Time::DeltaTime();

			if (mCreatureAttackDelayTime <= mCreatureAttackAccumulateTime)
			{
				mCreatureAttackReady = true;
				// �浹ü ���� ���ֱ�
				mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);
				mCreatureAttackAccumulateTime = 0.0f;
			}
		}
	}
	void EnemyWeaponScript::SetWeaponTransform(math::Vector3 ownerPos, math::Vector2 ownerDir)
	{		
		mEnemyWeaponTransform->SetPosition(ownerPos);

		bool isLeft = false;
		if (0 >= ownerDir.x)
			isLeft = true;
		mEnemyWeaponRenderer->SetReverse(isLeft);
	}
	void EnemyWeaponScript::DoAttack()
	{
		if (mCreatureAttackReady)
		{
			// ���� Ȱ��ȭ (Tr ����, ����Ʈ, �浹 ��ü ����)
			activeAttack();
			// ���� �ؽ��� & �ִϸ��̼� �����ϱ�

			mCreatureAttackReady = false;
		}
	}
	void EnemyWeaponScript::activeAttack()
	{
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSword", false);
		//�浹ü Ȱ��ȭ
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}
	void EnemyWeaponScript::OnCollisionEnter(Collider2D* other)
	{

		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{			
			// �ǰ� ȣ��
			GameDataManager::GetDamage(4.0f);
		}		
	}
}
