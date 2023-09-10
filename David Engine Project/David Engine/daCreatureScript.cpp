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
		, mCreatureRenderer(nullptr)
		, mCreatureRigidbody(nullptr)
		, mCreatureAnimator(nullptr)
		, mCreatureBodyCollider(nullptr)
		, mCreatureFootCollider(nullptr)
		, mCreatureStat{}
		, mCreatureDir(math::Vector2::Zero) 
		, mIsDead(false)
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



	void CreatureScript::VisualUpdate()
	{
		ReverseTexture();
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
	void CreatureScript::OnDamaged()
	{
		if (0 >= mCreatureStat->CurHP)
			mCreatureStat->CurHP = 0;
		mCreatureStat->CurHP -= 5.0f;
	}
}