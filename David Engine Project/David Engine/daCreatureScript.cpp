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
		
		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"SkelSprite", L"..\\Resources\\Texture\\Monster\\Skel\\SpriteSheet.png");
		std::shared_ptr<Texture> weapon = Resources::Load<Texture>(L"EnemyGreatSword", L"..\\Resources\\Texture\\Monster\\Skel\\GreatSwordSpriteSheet.png");
		mAnimator->Create(L"SkelIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->Create(L"SkelMove", texture, Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f), 6, Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->Create(L"SkelAttact", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->Create(L"SkelSwing", texture, Vector2(0.0f, 0.0f), Vector2(51.0f, 49.0f), 16, Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->PlayAnimation(L"SkelIdle");
	}
	EnemyWeaponScript* CreatureScript::SetEnemyWeaponScript(GameObject* creature)
	{
		EnemyWeaponScript* enemyWeaponScript = creature->AddComponent<EnemyWeaponScript>();
		return enemyWeaponScript;
	}
}