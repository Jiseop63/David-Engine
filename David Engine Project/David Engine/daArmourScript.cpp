#include "daArmourScript.h"

#include "daGameDataManager.h"
#include "daGameObject.h"
#include "daInput.h"
#include "daResources.h"
//#include "daMaterial.h"

namespace da
{
	ArmourScript::ArmourScript()
		: mPanelTransform(nullptr)
		, mPanelRenderer(nullptr)
		, mSlotScript(nullptr)
		, mIconScript(nullptr)
		, mPadding(0.20f)
		, mSwapTime(0.0f)
		, mBackup(false)
	{
	}
	ArmourScript::~ArmourScript()
	{
	}

	void ArmourScript::Initialize()
	{
		mPanelTransform = GetOwner()->GetComponent<Transform>();
		mPanelRenderer = GetOwner()->GetComponent<MeshRenderer>();

	}

	void ArmourScript::Update()
	{
		if (mBackup)
		{
			int a = 0;
		}
		else
		{
			int a = 0;
		}

		/*if (Input::GetKeyDown(eKeyCode::TILDE))
		{
			ChangeArmour();
		}*/
	}

	void ArmourScript::AddIconScript(GameObject* iconObject)
	{
		mIconScript = iconObject->AddComponent<ItemIconScript>();
	}

	void ArmourScript::ChangeArmour()
	{
		// 교체 기능
		math::Vector3 retPosition = mPanelTransform->GetPosition();

		// 전방으로 이동
		if (mBackup)
		{
			mBackup = false;
			retPosition += math::Vector3(-mPadding, -mPadding, -0.0001f);
		}
		// 후방으로 이동
		else
		{
			mBackup = true;
			retPosition += math::Vector3(mPadding, mPadding, 0.0001f);
		}
		mPanelTransform->SetPosition(retPosition);
		ClearPosition();
	}

	void ArmourScript::ChangeIcon()
	{
		if (mSlotScript->HasItem())
		{
			mIconScript->SetIconTexture(mSlotScript->GetItemScript()->GetItemTexture());
			mIconScript->SetIconScale(mSlotScript->GetItemScript()->GetItemScale());
			ClearPosition();
		}
		else
		{
			mIconScript->SetIconTexture(nullptr);
			ClearPosition();
		}
	}

	void ArmourScript::ClearPosition()
	{
		math::Vector3 panelPos = mPanelTransform->GetPosition();
		panelPos.z += -0.00001f;
		mIconScript->SetIconPosition(panelPos);
	}

}