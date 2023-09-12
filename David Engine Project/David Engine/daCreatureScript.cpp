#include "daCreatureScript.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daSceneManager.h"
#include "daEffectScript.h"
#include "daTime.h"

namespace da
{
	using namespace math;

	CreatureScript::CreatureScript()
		: mCreatureTransform(nullptr)
		, mCreatureRenderer(nullptr)
		, mCreatureRigidbody(nullptr)
		, mCreatureAnimator(nullptr)
		, mCreatureBodyCollider(nullptr)
		, mCreatureFootCollider(nullptr)
		, mCreatureStat{}
		, mCreatureDir(math::Vector2::Zero) 
		, mIsDead(false)
		, mBodyCollision(false)
		, mFootCollision(false)
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
		mCreatureBodyCollider->ImBody();
		mCreatureBodyCollider->SetName(L"BodyCollider");
		mCreatureBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		GetOwner()->SetBodyCollider(mCreatureBodyCollider);

		mCreatureFootCollider = GetOwner()->AddComponent<Collider2D>();
		mCreatureFootCollider->ImFoot();
		mCreatureFootCollider->SetName(L"FootCollider");
		mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		GetOwner()->SetFootCollider(mCreatureFootCollider);
	}

	EffectScript* CreatureScript::callEffect()
	{
		for (size_t effect = 0; effect < mEffects.size(); effect++)
		{
			if (GameObject::eObjectState::Inactive ==
				mEffects[effect]->GetOwner()->GetObjectState())
				return mEffects[effect];
		}
		return nullptr;
	}

	void CreatureScript::visualUpdate()
	{
		reverseTexture();
	}

	void CreatureScript::moveLeft()
	{
		Vector3 myPos = mCreatureTransform->GetPosition();
		float moveMagnitude = mCreatureStat.MoveSpeed * (float)Time::DeltaTime();
		Collider2D::eWallCollisionState wallCollisionState = mCreatureBodyCollider->IsWallCollision();
		if (Collider2D::eWallCollisionState::Left == wallCollisionState)
			return;
		else
		{
			Vector2 moveDir = daRotateVector2(-Vector2::UnitX, mCreatureFootCollider->GetEnvRotate());
			Vector2 movePosition = moveDir * moveMagnitude;

			myPos.x += movePosition.x;
			myPos.y += movePosition.y;
		}
		mCreatureTransform->SetPosition(myPos);
	}

	void CreatureScript::moveRight()
	{
		Vector3 myPos = mCreatureTransform->GetPosition();
		float moveMagnitude = mCreatureStat.MoveSpeed * (float)Time::DeltaTime();
		Collider2D::eWallCollisionState wallCollisionState = mCreatureBodyCollider->IsWallCollision();

		if (Collider2D::eWallCollisionState::Right == wallCollisionState)
			return;
		else
		{
			Vector2 moveDir = daRotateVector2(Vector2::UnitX, mCreatureFootCollider->GetEnvRotate());
			Vector2 movePosition = moveDir * moveMagnitude;

			myPos.x += movePosition.x;
			myPos.y += movePosition.y;
		}

		mCreatureTransform->SetPosition(myPos);
	}

	void CreatureScript::OnCollisionEnter(Collider2D* other)
	{
		// 플렛폼에 충돌
		if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
		{
			// 어디가 충돌했는지 확인하기
			if (mCreatureBodyCollider->IsCollision())
				mBodyCollision = false;
			if (mCreatureFootCollider->IsCollision())
				mFootCollision = false;
		}
	}
	void CreatureScript::OnDamaged(float damage)
	{
		mCreatureStat.CurHP -= damage;
		if (0 >= mCreatureStat.CurHP)
			mCreatureStat.CurHP = 0;
	}
}