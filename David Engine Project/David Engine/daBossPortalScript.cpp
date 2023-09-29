#include "daBossPortalScript.h"
#include "daResources.h"
#include "daInput.h"

#include "daGameObject.h"
#include "daPortalIconScript.h"

namespace da
{
	BossPortalScript::BossPortalScript()
		: mPortalIcon(nullptr)
		, mIsClear(false)
		, mFocused(false)
	{
	}
	BossPortalScript::~BossPortalScript()
	{
	}
	void BossPortalScript::Initialize()
	{
		PortalScript::Initialize();
		mPortalTransform->SetScale(math::Vector3(4.0f, 4.0f, 1.0f));

		mPortalAnimator->Create(L"Stage1DoorClosing", Resources::Find<Texture>(L"Stage1DoorClosing")
			, math::Vector2::Zero, math::Vector2(57.0f, 65.0f), 10, math::Vector2::Zero, 0.10f, 100.0f);
		mPortalAnimator->Create(L"Stage1DoorOpening", Resources::Find<Texture>(L"Stage1DoorOpening")
			, math::Vector2::Zero, math::Vector2(57.0f, 65.0f), 10, math::Vector2::Zero, 0.10f, 100.0f);
		mPortalAnimator->Create(L"Stage1DoorClose", Resources::Find<Texture>(L"Stage1DoorClose")
			, math::Vector2::Zero, math::Vector2(57.0f, 65.0f), 1, math::Vector2::Zero, 0.10f, 100.0f);
		mPortalAnimator->Create(L"Stage1DoorOpen", Resources::Find<Texture>(L"Stage1DoorOpen")
			, math::Vector2::Zero, math::Vector2(57.0f, 65.0f), 1, math::Vector2::Zero, 0.10f, 100.0f);

		//mPortalAnimator->CompleteEvent(L"SteleClose") = std::bind(&DungeonPortalScript::IdleAnimation, this);
		mPortalAnimator->CompleteEvent(L"Stage1DoorClosing") = std::bind(&BossPortalScript::PlayDoorClose, this);
		mPortalAnimator->CompleteEvent(L"Stage1DoorOpening") = std::bind(&BossPortalScript::PlayDoorOpen, this);
		mPortalAnimator->ActionEvent(L"Stage1DoorOpening", 4) = std::bind(&BossPortalScript::HidePlayer, this);

		// 기본값은 충돌 가능한 상태
		mPortalBodyCollider->SetSize(math::Vector2(0.570f, 0.650f));
		mPortalBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}
	void BossPortalScript::Update()
	{
		if (mIsClear && mFocused)
		{
			if (Input::GetKeyDown(eKeyCode::F))
			{
				PortalScript::ChangeScene();
			}
		}
	}

	void BossPortalScript::OnCollisionEnter(Collider2D* other)
	{		
		if (mIsClear)
		{
			mFocused = true;
			mPortalIcon->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		}
	}
	void BossPortalScript::OnCollisionExit(Collider2D* other)
	{
		if (mIsClear)
		{
			mFocused = false;
			mPortalIcon->ApplyIcon(false);
			mPortalIcon->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		}
	}
	void BossPortalScript::SetPosition(math::Vector3 position)
	{
		mPortalTransform->SetPosition(position);
		mPortalIcon->SetPosition(position);
	}
	void BossPortalScript::AddIconScript(GameObject* object)
	{
		mPortalIcon = object->AddComponent<PortalIconScript>();
	}
	void BossPortalScript::GatePass()
	{
	}
	void BossPortalScript::GateClose()
	{
		mIsClear = false;
		mPortalBodyCollider->ApplyComponentUsing(false);
		mPortalAnimator->PlayAnimation(L"Stage1DoorClosing", false);
		mPortalIcon->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
	void BossPortalScript::GateOpen()
	{
		mPortalAnimator->PlayAnimation(L"Stage1DoorOpening", false);
	}
	void BossPortalScript::DoorOpening()
	{
		mPortalAnimator->PlayAnimation(L"Stage1DoorOpening", false);
	}
	void BossPortalScript::DoorClosing()
	{
		mIsClear = false;
		mPortalBodyCollider->ApplyComponentUsing(false);
		mPortalAnimator->PlayAnimation(L"Stage1DoorClosing", false);
	}


	void BossPortalScript::PlayDoorOpen()
	{
		mIsClear = true;
		mPortalAnimator->PlayAnimation(L"Stage1DoorOpen", false);
		mPortalBodyCollider->ApplyComponentUsing(true);
	}
	void BossPortalScript::PlayDoorClose()
	{
		mPortalAnimator->PlayAnimation(L"Stage1DoorClose", false);
	}
	void BossPortalScript::HidePlayer()
	{
		SetPosition(mPortalTransform->GetPosition() + math::Vector3(0.0f, 0.0f, -1.0f));
	}
}
