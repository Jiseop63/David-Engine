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
		GetOwner()->SetBodyCollider(mCreatureBodyCollider);
		mCreatureFootCollider = GetOwner()->AddComponent<Collider2D>();
		GetOwner()->SetFootCollider(mCreatureFootCollider);


		// 콜라이더 세팅은 각자 알아서 하는거로 하자..?
		mCreatureBodyCollider->ImBody();
		mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mCreatureFootCollider->ImFoot();
		mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		
		// 이건 Creature를 상속받는 Monster에서 진행하도록 하자
		/*mCreatureSensorCollider = GetOwner()->AddComponent<Collider2D>();*/
		/*mCreatureSensorCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
		mCreatureSensorCollider->SetSize(Vector2(4.50f, 1.90f));
		mCreatureFootCollider->SetCenter(Vector2(0.0f, 0.450f));*/


		// stat 세팅... 은 알아서 하세요

			
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