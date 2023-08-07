#include "daCreatureScript.h"
#include "daCreatureScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daSceneManager.h"
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
		, mPlayerScript(nullptr)
		, mCreatureStat{}
		, mCreatureActiveState(eCreatureState::Idle)
		, mIsDead(false)
	{
	}
	CreatureScript::~CreatureScript()
	{
	}
	void CreatureScript::Initialize()
	{
		// component ����
		mCreatureTransform = GetOwner()->GetComponent<Transform>();
		mCreatureRigidbody = GetOwner()->AddComponent<Rigidbody>();
		mCreatureAnimator = GetOwner()->AddComponent<Animator>();
		mCreatureBodyCollider = GetOwner()->AddComponent<Collider2D>();
		mCreatureFootCollider = GetOwner()->AddComponent<Collider2D>();
		GetOwner()->SetFootCollider(mCreatureFootCollider);

		mCreatureBodyCollider->SetSize(Vector2(0.30f, 0.40f));
		mCreatureBodyCollider->SetCenter(Vector2(0.0f, -0.10f));

		mCreatureFootCollider->SetSize(Vector2(0.050f, 0.050f));
		mCreatureFootCollider->SetCenter(Vector2(0.0f, -0.450f));
		mCreatureFootCollider->SetColliderDetection(Collider2D::eColliderDetection::Land);
		
		mPlayerScript = SceneManager::GetPlayerScript();

		// stat ����

		mCreatureStat.MaxHP = 20.0f;
		mCreatureStat.CurHP = 20.0f;
		
		mCreatureStat.MoveSpeed = 0.750f;
		mCreatureStat.DetectRange = 2.50f;

		mCreatureStat.AttackRange = 1.250f;
		mCreatureStat.AttackAccumulateTime = 0.0f;
		mCreatureStat.AttackDelay = 3.0f;		
	}

	EnemyWeaponScript* CreatureScript::SetEnemyWeaponScript(GameObject* creature)
	{
		EnemyWeaponScript* enemyWeaponScript = creature->AddComponent<EnemyWeaponScript>();
		return enemyWeaponScript;
	}
	EffectEnemyScript* CreatureScript::AddEffectObject(GameObject* effectObject)
	{
		mEnemyEffectScript = effectObject->AddComponent<EffectEnemyScript>();
		return mEnemyEffectScript;
	}
	void CreatureScript::OnDamaged()
	{
		if (0 >= mCreatureStat.CurHP)
			mCreatureStat.CurHP = 0;
		
		mCreatureStat.CurHP -= 5.0f;
	}
}