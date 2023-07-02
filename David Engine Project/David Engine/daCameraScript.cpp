#include "daCameraScript.h"

#include "daTime.h"
#include "daInput.h"

#include "daGameObject.h"
#include "daTransform.h"
#include "daCamera.h"

namespace da
{
	CameraScript::CameraScript()
		: mCameraComponent(nullptr)
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
		mCameraComponent = GetOwner()->GetComponent<Camera>();
	}

	void CameraScript::Update()
	{
		Transform* myTransform = GetOwner()->GetComponent<Transform>();
		Vector3 pos = myTransform->GetPosition();
		float viewSize = mCameraComponent->GetSize();
		Camera::eProjectionType projectionType = mCameraComponent->GetProjectionType();

		if (Input::GetKey(eKeyCode::NUM_9))
		{
			pos += myTransform->Forward() * 5.0f * (float)Time::DeltaTime();			
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_3))
		{
			pos += -myTransform->Forward() * 5.0f * (float)Time::DeltaTime();
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_4))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += -myTransform->Right() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += -myTransform->Right() * -pos.z * (float)Time::DeltaTime();
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_6))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += myTransform->Right() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += myTransform->Right() * -pos.z * (float)Time::DeltaTime();
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_8))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += myTransform->Up() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += myTransform->Up() * -pos.z * (float)Time::DeltaTime();
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_2))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += -myTransform->Up() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += -myTransform->Up() * -pos.z * (float)Time::DeltaTime();
			myTransform->SetPosition(pos);
		}
		// num 0 : ProjectionType Change
		else if (Input::GetKeyDown(eKeyCode::NUM_0))
		{
			Camera::eProjectionType projectionType = mCameraComponent->GetProjectionType();

			if (Camera::eProjectionType::Orthographic == projectionType)
				mCameraComponent->SetProjectionType(Camera::eProjectionType::Perspective);
			else
				mCameraComponent->SetProjectionType(Camera::eProjectionType::Orthographic);
		}

		// num 7 - 1 : size Up-Down
		else if (Input::GetKey(eKeyCode::NUM_7))
		{			
			viewSize -= 1.f * (float)Time::DeltaTime();
			if (0.001 <= viewSize)
				mCameraComponent->SetSize(viewSize); 
		}
		else if (Input::GetKey(eKeyCode::NUM_1))
		{
			viewSize += 1.f * (float)Time::DeltaTime();
			mCameraComponent->SetSize(viewSize);
		}

		// num 5 : Debug On-Off

	}
}
