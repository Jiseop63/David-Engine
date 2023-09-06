#include "daCreatureScript.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daSceneManager.h"
#include "daEnemyWeaponScript.h"
#include "daEffectEnemyScript.h"
#include "daCreatureLifebarScript.h"

namespace da
{
	using namespace math;

	CreatureScript::CreatureScript()
		: mCreatureTransform(nullptr)
		, mCreatureRigidbody(nullptr)
		, mCreatureAnimator(nullptr)
		, mCreatureBodyCollider(nullptr)
		, mCreatureFootCollider(nullptr)

		, mCreatureWeaponScript(nullptr)
		, mEnemyEffectScript(nullptr)
		, mCreatureLifeScript(nullptr)
		, mPlayerScript(nullptr)
		, mCreatureStat{}
		, mCreatureActiveState(eCreatureState::Idle)
		, mStandingPosition(math::Vector3::Zero) 
		, mCreatureDir(math::Vector2::Zero) 
		, mIsDead(false)
		, mDetectPlayer(false)
	{
	}
	CreatureScript::~CreatureScript()
	{
	}
	void CreatureScript::Initialize()
	{
		// component ����
		mCreatureTransform = GetOwner()->GetComponent<Transform>();
		mCreatureRenderer = GetOwner()->GetComponent <MeshRenderer>();
		mCreatureRigidbody = GetOwner()->AddComponent<Rigidbody>();
		mCreatureAnimator = GetOwner()->AddComponent<Animator>();
		mCreatureBodyCollider = GetOwner()->AddComponent<Collider2D>();
		GetOwner()->SetBodyCollider(mCreatureBodyCollider);
		mCreatureFootCollider = GetOwner()->AddComponent<Collider2D>();
		GetOwner()->SetFootCollider(mCreatureFootCollider);


		// �ݶ��̴� ������ ���� �˾Ƽ� �ϴ°ŷ� ����..?
		mCreatureBodyCollider->ImBody();
		mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mCreatureFootCollider->ImFoot();
		mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		
		// �̰� Creature�� ��ӹ޴� Monster���� �����ϵ��� ����
		/*mCreatureSensorCollider = GetOwner()->AddComponent<Collider2D>();*/
		/*mCreatureSensorCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
		mCreatureSensorCollider->SetSize(Vector2(4.50f, 1.90f));
		mCreatureFootCollider->SetCenter(Vector2(0.0f, 0.450f));*/


		// stat ����... �� �˾Ƽ� �ϼ���

			
	}

	void CreatureScript::ReverseTexture()
	{
		bool value = IsLeft();
		mCreatureRenderer->SetReverse(value);
		mCreatureWeaponScript->SetReverse(value);
	}

	EnemyWeaponScript* CreatureScript::SetEnemyWeaponScript(GameObject* creature)
	{
		mCreatureWeaponScript = creature->AddComponent<EnemyWeaponScript>();
		return mCreatureWeaponScript;
	}
	CreatureLifebarScript* CreatureScript::SetCreatureLifeScript(GameObject* creature)
	{
		mCreatureLifeScript = creature->AddComponent<CreatureLifebarScript>();
		mCreatureLifeScript->SetCreatureScript(this);
		mCreatureLifeScript->SetValue(mCreatureStat.MaxHP, mCreatureStat.CurHP);
		return mCreatureLifeScript;
	}
	EffectEnemyScript* CreatureScript::AddEffectObject(GameObject* effectObject)
	{
		mEnemyEffectScript = effectObject->AddComponent<EffectEnemyScript>();
		return mEnemyEffectScript;
	}
	void CreatureScript::OnCollisionEnter(Collider2D* other)
	{
		// �÷����� �浹
		if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
		{
			// ��� �浹�ߴ��� Ȯ���ϱ�
			if (mCreatureBodyCollider->IsCollision())
				mBodyCollision = false;
			if (mCreatureFootCollider->IsCollision())
				mFootCollision = false;
		}
	}
	void CreatureScript::OnDamaged()
	{
		if (0 >= mCreatureStat.CurHP)
			mCreatureStat.CurHP = 0;
		mCreatureStat.CurHP -= 5.0f;

		mCreatureLifeScript->SetValue(mCreatureStat.MaxHP, mCreatureStat.CurHP);
	}
}