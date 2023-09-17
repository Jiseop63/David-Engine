#include "daFlyingCreatureScript.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daSceneManager.h"
#include "daEffectScript.h"
#include "daTime.h"

namespace da
{
	FlyingCreatureScript::FlyingCreatureScript()
		: mCreatureTransform(nullptr)
		, mCreatureRenderer(nullptr)
		, mCreatureRigidbody(nullptr)
		, mCreatureAnimator(nullptr)
		, mCreatureBodyCollider(nullptr)
		, mIsDead(false)
	{
	}
	FlyingCreatureScript::~FlyingCreatureScript()
	{
	}
	void FlyingCreatureScript::Initialize()
	{
		// component ¼¼ÆÃ
		mCreatureTransform = GetOwner()->GetComponent<Transform>();
		mCreatureRenderer = GetOwner()->GetComponent <MeshRenderer>();
		mCreatureRigidbody = GetOwner()->AddComponent<Rigidbody>();
		mCreatureAnimator = GetOwner()->AddComponent<Animator>();

		mCreatureBodyCollider = GetOwner()->AddComponent<Collider2D>();
		mCreatureBodyCollider->ImBody();
		mCreatureBodyCollider->SetName(L"BodyCollider");
		mCreatureBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		GetOwner()->SetBodyCollider(mCreatureBodyCollider);
		mCreatureRigidbody->GravityAble(false);
	}
	EffectScript* FlyingCreatureScript::callEffect()
	{
		for (size_t effect = 0; effect < mEffects.size(); effect++)
		{
			if (GameObject::eObjectState::Inactive ==
				mEffects[effect]->GetOwner()->GetObjectState())
				return mEffects[effect];
		}
		return nullptr;
	}
	void FlyingCreatureScript::visualUpdate()
	{
		reverseTexture();
	}
	void FlyingCreatureScript::moveLeft()
	{
		math::Vector3 myPos = mCreatureTransform->GetPosition();
		float moveMagnitude = mCreatureStat.MoveSpeed * (float)Time::DeltaTime();
		Collider2D::eWallCollisionState wallCollisionState = mCreatureBodyCollider->IsWallCollision();
		if (Collider2D::eWallCollisionState::Left == wallCollisionState)
			return;
		else
		{
			//math::Vector2 moveDir = math::daRotateVector2(-math::Vector2::UnitX, mCreatureFootCollider->GetEnvRotate());
			math::Vector2 movePosition = -math::Vector2::UnitX * moveMagnitude;

			myPos.x += movePosition.x;
			myPos.y += movePosition.y;
		}
		mCreatureTransform->SetPosition(myPos);
	}
	void FlyingCreatureScript::moveRight()
	{
		math::Vector3 myPos = mCreatureTransform->GetPosition();
		float moveMagnitude = mCreatureStat.MoveSpeed * (float)Time::DeltaTime();
		Collider2D::eWallCollisionState wallCollisionState = mCreatureBodyCollider->IsWallCollision();

		if (Collider2D::eWallCollisionState::Right == wallCollisionState)
			return;
		else
		{
			//math::Vector2 moveDir = daRotateVector2(Vector2::UnitX, mCreatureFootCollider->GetEnvRotate());
			math::Vector2 movePosition = math::Vector2::UnitX * moveMagnitude;

			myPos.x += movePosition.x;
			myPos.y += movePosition.y;
		}

		mCreatureTransform->SetPosition(myPos);
	}
	void FlyingCreatureScript::OnDamaged(float damage)
	{
		mCreatureStat.CurHP -= damage;
		if (0 >= mCreatureStat.CurHP)
			mCreatureStat.CurHP = 0;
	}
}
