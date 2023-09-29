#include "daDungeonPortalScript.h"
#include "daResources.h"
#include "daGameDataManager.h"
#include "daPortalWallScript.h"

namespace da
{
	DungeonPortalScript::DungeonPortalScript()
		: mWallScript(nullptr)
		, mIsClear(true)
	{
	}
	DungeonPortalScript::~DungeonPortalScript()
	{
	}
	void DungeonPortalScript::Initialize()
	{
		PortalScript::Initialize();
		mPortalTransform->SetScale(math::Vector3(8.0f, 8.0f, 1.0f));

		mPortalAnimator->Create(L"SteleClose", Resources::Find<Texture>(L"SteleClose")
			, math::Vector2::Zero, math::Vector2(66.0f, 20.0f), 8, math::Vector2::Zero, 0.10f, 200.0f);
		mPortalAnimator->Create(L"SteleIdle", Resources::Find<Texture>(L"SteleIdle")
			, math::Vector2::Zero, math::Vector2(66.0f, 20.0f), 8, math::Vector2::Zero, 0.10f, 200.0f);
		mPortalAnimator->Create(L"SteleOpen", Resources::Find<Texture>(L"SteleOpen")
			, math::Vector2::Zero, math::Vector2(66.0f, 20.0f), 8, math::Vector2::Zero, 0.10f, 200.0f);

		mPortalAnimator->CompleteEvent(L"SteleClose") = std::bind(&DungeonPortalScript::IdleAnimation, this);
		mPortalAnimator->CompleteEvent(L"SteleOpen") = std::bind(&DungeonPortalScript::PortalActive, this);

		// 기본값은 충돌 가능한 상태
		mPortalRenderer->SetMaterial(Resources::Find<Material>(L"NoneMaterial"));
		mPortalBodyCollider->SetSize(math::Vector2(0.30f, 0.060f));
		mPortalBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}
	void DungeonPortalScript::Update()
	{
		
	}

	void DungeonPortalScript::OnCollisionEnter(Collider2D* other)
	{
		if (mIsClear)
			PortalScript::ChangeScene();
	}
	
	void DungeonPortalScript::SetRotate(float angle)
	{
		mPortalTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		mWallScript->SetColliderSize(math::Vector2(0.50f, 2.40f));
	}

	void DungeonPortalScript::SetPosition(math::Vector3 position)
	{
		mPortalTransform->SetPosition(position);
		mWallScript->SetPosition(position);
	}

	void DungeonPortalScript::AddWallScript(GameObject* object)
	{
		mWallScript = object->AddComponent<PortalWallScript>();
	}


	void DungeonPortalScript::GatePass()
	{
		PortalActive();
	}

	void DungeonPortalScript::GateOpen()
	{
		SetMaterial(Resources::Find<Material>(L"AnimationMaterial"));
		mPortalAnimator->PlayAnimation(L"SteleOpen", false);
	}
	void DungeonPortalScript::GateClose()
	{
		mIsClear = false;
		mWallScript->ApplyCollision();
		mPortalBodyCollider->ApplyComponentUsing(false);
		SetMaterial(Resources::Find<Material>(L"AnimationMaterial"));
		mPortalAnimator->PlayAnimation(L"SteleClose", false);
	}


	void DungeonPortalScript::IdleAnimation()
	{
		mPortalAnimator->PlayAnimation(L"SteleIdle");
	}
	void DungeonPortalScript::PortalActive()
	{
		mIsClear = true;
		mWallScript->ApplyCollision(false);
		mPortalBodyCollider->ApplyComponentUsing();
		SetMaterial(Resources::Find<Material>(L"NoneMaterial"));
	}
}