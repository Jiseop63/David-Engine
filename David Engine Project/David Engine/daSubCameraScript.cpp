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
		// 메인카메라 좌표 받기
		math::Vector3 mainCamPos = mMainCameraTransform->GetPosition();

		// 내 좌표 구하기
		Transform* myTransform = GetOwner()->GetTransform();
		math::Vector3 myPos = myTransform->GetPosition();

		// x값만 적용하기
		myPos.y = mainCamPos.y;
		myTransform->SetPosition(myPos);
	}
}
