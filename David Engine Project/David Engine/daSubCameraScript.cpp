#include "daSubCameraScript.h"
#include "daGameObject.h"

namespace da
{
	SubCameraScript::SubCameraScript()
		: mMainCameraTransform(nullptr)
	{
	}
	SubCameraScript::~SubCameraScript()
	{
	}
	void SubCameraScript::Update()
	{
		// ����ī�޶� ��ǥ �ޱ�
		math::Vector3 mainCamPos = mMainCameraTransform->GetPosition();

		// �� ��ǥ ���ϱ�
		Transform* myTransform = GetOwner()->GetTransform();
		math::Vector3 myPos = myTransform->GetPosition();

		// x���� �����ϱ�
		myPos.y = mainCamPos.y;
		myTransform->SetPosition(myPos);
	}
}
