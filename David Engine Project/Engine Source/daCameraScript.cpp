#include "daCameraScript.h"
#include "daTransform.h"
#include "daGameObject.h"
#include "daTime.h"
#include "daInput.h"

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
		Vector3 pos = tr->GetPosition();
		if (Input::GetKey(eKeyCode::NUM_9))
		{
			pos += tr->Forward() * 5.0f * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_3))
		{
			pos += -tr->Forward() * 5.0f * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_4))
		{
			pos += -tr->Right() * 5.0f * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_6))
		{
			pos += tr->Right() * 5.0f * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_8))
		{
			pos += -tr->Up() * 5.0f * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_2))
		{
			pos += tr->Up() * 5.0f * (float)Time::DeltaTime();
			tr->SetPosition(pos);
		}
		
		// num 5 : Debug On-Off
		// num 7 - 1 : size Up-Down
		// num 0 : ProjectionType Change
	}
}
