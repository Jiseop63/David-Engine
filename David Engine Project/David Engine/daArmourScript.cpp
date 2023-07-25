#include "daArmourScript.h"

#include "daGameDataManager.h"
#include "daGameObject.h"
#include "daInput.h"
#include "daResources.h"
//#include "daMaterial.h"

namespace da
{
	ArmourScript::ArmourScript()
		: mTransform(nullptr)
		, mPanelRenderer(nullptr)
		, mIconRenderer(nullptr)
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
		mTransform = GetOwner()->GetComponent<Transform>();
		mPanelRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mIconRenderer = GetOwner()->AddComponent<MeshRenderer>();

		mIconRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		std::shared_ptr<Material> material
			= Resources::Find<Material>(L"WeaponIconMaterial");
		mIconRenderer->SetMaterial(material);

	}

	void ArmourScript::Update()
	{
		if (Input::GetKeyDown(eKeyCode::TILDE))
		{
			ChangeArmour();
		}
	}
	void ArmourScript::LateUpdate()
	{
	}

	void ArmourScript::ChangeArmour()
	{
		// ������ ����
		GameDataManager::ChangeArmour();

		// ��ü ���
		math::Vector3 getPos = mTransform->GetPosition();

		// �������� �̵�
		if (mBackup)
		{
			mBackup = false;
			getPos += math::Vector3(-mPadding, -mPadding, -0.0001f);
		}
		// �Ĺ����� �̵�
		else
		{
			mBackup = true;
			getPos += math::Vector3(mPadding, mPadding, 0.0001f);
		}
		mTransform->SetPosition(getPos);
	}

}