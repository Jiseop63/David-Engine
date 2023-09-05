#include "daCreatureScript.h"
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
		// component 세팅
		mCreatureTransform = GetOwner()->GetComponent<Transform>();
		mCreatureRenderer = GetOwner()->GetComponent <MeshRenderer>();
		mCreatureRigidbody = GetOwner()->AddComponent<Rigidbody>();
		mCreatureAnimator = GetOwner()->AddComponent<Animator>();
		mCreatureBodyCollider = GetOwner()->AddComponent<Collider2D>();
		GetOwner()->SetBodyCollider(mCreatureBodyCollider);
		mCreatureFootCollider = GetOwner()->AddComponent<Collider2D>();
		GetOwner()->SetFootCollider(mCreatureFootCollider);
		mCreatureSensorCollider = GetOwner()->AddComponent<Collider2D>();

		mCreatureBodyCollider->ImBody();
		mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mCreatureBodyCollider->SetSize(Vector2(0.30f, 0.60f));
		mCreatureBodyCollider->SetCenter(Vector2(0.0f, -0.10f));


		mCreatureFootCollider->ImFoot();
		mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mCreatureFootCollider->SetSize(Vector2(0.050f, 0.050f));
		mCreatureFootCollider->SetCenter(Vector2(0.0f, -0.450f));
		

		mCreatureSensorCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
		mCreatureSensorCollider->SetSize(Vector2(3.50f, 0.90f));

		mPlayerScript = SceneManager::GetPlayerScript();

		// stat 세팅

		mCreatureStat.MaxHP = 25.0f;
		mCreatureStat.CurHP = 25.0f;
		
		mCreatureStat.MoveSpeed = 1.250f;
		mCreatureStat.DetectRange = 2.50f;

		mCreatureStat.AttackRange = 0.750f;
		mCreatureStat.AttackAccumulateTime = 0.0f;
		mCreatureStat.AttackDelay = 4.0f;		
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
	void CreatureScript::OnDamaged()
	{
		if (0 >= mCreatureStat.CurHP)
			mCreatureStat.CurHP = 0;
		mCreatureStat.CurHP -= 5.0f;

		mCreatureLifeScript->SetValue(mCreatureStat.MaxHP, mCreatureStat.CurHP);
	}
}