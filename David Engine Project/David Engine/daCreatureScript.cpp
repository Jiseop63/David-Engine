#include "daCreatureScript.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daSceneManager.h"
#include "daTime.h"

#include "daActionUnitScript.h"
#include "daAudioSource.h"

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
		, mCreatureAudio(nullptr)
		, mCreatureStat{}
		, mCreatureDir(math::Vector2::Zero) 
		, mIsDead(false)
		, mBodyCollision(false)
		, mFootCollision(false)
		, mIsControl(true)
		, mHitEffectAngle(0.0f)
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
		mCreatureAudio = GetOwner()->AddComponent<AudioSource>();
		
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

	ActionUnitScript* CreatureScript::callActionUnit()
	{
		for (size_t unit = 0; unit < mActionUnits.size(); unit++)
		{
			if (GameObject::eObjectState::Active ==
				mActionUnits[unit]->GetOwner()->GetObjectState())
				continue;
			return mActionUnits[unit];
		}
		return nullptr;
	}

	void CreatureScript::InactiveObejct()
	{
		GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
	}

	void CreatureScript::ActiveObejct()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
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
	void CreatureScript::CallHitEffect(math::Vector3 position)
	{
		//if (7 <= mHitEffectAngle)
		//	mHitEffectAngle = 0.0f;
		//mHitEffectAngle += 1.80f;
		//// 방향 구하기
		//ActionUnitScript* actionUnit = callActionUnit();

		//structs::sActionUnitTypes effectUnitTypes = {};
		//effectUnitTypes.Usage = enums::eUnitUsageType::JustAnimation;
		//effectUnitTypes.LifeCycle = enums::eUnitLifeType::AnimationEnd;
		//effectUnitTypes.Action = enums::eUnitActionType::Stay;
		//actionUnit->SetUnitTypes(effectUnitTypes);

		//structs::sActionUnitStat effectUnitInfo = {};
		//effectUnitInfo.Animation.Action = L"Slash";
		//actionUnit->SetUnitInfo(effectUnitInfo);
		//actionUnit->SetUnitRotateAngle(mHitEffectAngle);
		//actionUnit->SetUnitReverse(isLeft());
		//actionUnit->SetUnitScale(math::Vector3(1.750f, 1.750f, 1.0f));
		//actionUnit->SetUnitOffset(math::Vector3(0.0f, -0.20f, 0.0f));
		//actionUnit->OnActive();
		//actionUnit->SetUnitOverridePosition(position);
	}
}