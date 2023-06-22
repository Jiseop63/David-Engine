#include "daCameraScript.h"
#include "daTransform.h"
#include "daGameObject.h"
#include "daTime.h"

namespace da
{
	CameraScript::CameraScript()
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		math::Vector3 pos = tr->GetPosition();
		pos.x += 1.0f * (float)Time::DeltaTime();
		tr->SetPosition(pos);
	}
}
