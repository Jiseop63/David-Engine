#include "daCreatureScript.h"
#include "daGameObject.h"
#include "daResources.h"

namespace da
{
	using namespace math;

	CreatureScript::CreatureScript()
		: mTransform(nullptr)
		, mRigidbody(nullptr)
		, mAnimator(nullptr)
		, mBodyCollider(nullptr)
		, mFootCollider(nullptr)

		, mWeaponScript(nullptr)
	{
	}
	CreatureScript::~CreatureScript()
	{
	}
	void CreatureScript::Initialize()
	{
		// component ¼¼ÆÃ
		mTransform = GetOwner()->GetComponent<Transform>();
		mRigidbody = GetOwner()->AddComponent<Rigidbody>();
		mAnimator = GetOwner()->AddComponent<Animator>();
		mBodyCollider = GetOwner()->AddComponent<Collider2D>();
		mFootCollider = GetOwner()->AddComponent<Collider2D>();
		GetOwner()->SetFootCollider(mFootCollider);

		mBodyCollider->SetSize(Vector2(0.30f, 0.40f));
		mBodyCollider->SetCenter(Vector2(0.0f, -0.10f));

		mFootCollider->SetSize(Vector2(0.050f, 0.050f));
		mFootCollider->SetCenter(Vector2(0.0f, -0.450f));
		mFootCollider->SetColliderDetection(Collider2D::eColliderDetection::Land);
		
		
	}
	EnemyWeaponScript* CreatureScript::SetEnemyWeaponScript(GameObject* creature)
	{
		EnemyWeaponScript* enemyWeaponScript = creature->AddComponent<EnemyWeaponScript>();
		return enemyWeaponScript;
	}
}