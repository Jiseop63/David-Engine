#include "daCameraScript.h"

#include "daRenderer.h"
#include "daTime.h"
#include "daInput.h"

#include "daGameObject.h"
#include "daTransform.h"
#include "daCamera.h"

#include "daGameDataManager.h"


namespace da
{
	CameraScript::CameraScript()
		: mCameraComponent(nullptr)
		, mCameraShaking(false)
		, mOriginPosition(math::Vector3::Zero)
		, mOscillationPower(0.0f)
		, mShakeAccumulateTime(0.0f)
		, mShakeValidTime(0.0f)
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Update()
	{
		moveCamera();
		CameraShake();
		debugInputCamera();
	}
	void CameraScript::moveCamera()
	{
		if (mCameraShaking)
			return;

		// 내 위치
		Transform* myTransform = GetOwner()->GetComponent<Transform>();
		Vector3 pos = myTransform->GetPosition();
		// 최대 이동가능한 거리
		math::Vector2 maxRange = GameDataManager::GetCameraMovableRange();
		// 이동시킨 카메라 위치
		math::Vector2 currentPosition = GameDataManager::GetCameraMovaPosition();
		
		// 이동시킬 위치에 따른 변경할 위치값 설정
		if (currentPosition.x <= maxRange.x
			&& currentPosition.x >= -maxRange.x)
			pos.x = currentPosition.x;
		else if (-1 >= currentPosition.x)
			pos.x = -maxRange.x;
		else if (1 <= currentPosition.x)
			pos.x = maxRange.x;

		if (currentPosition.y <= maxRange.y
			&& currentPosition.y >= -maxRange.y)
			pos.y = currentPosition.y;
		else if (-1 >= currentPosition.y)
			pos.y = -maxRange.y;
		else if (1 <= currentPosition.y)
			pos.y = maxRange.y;

		mOriginPosition = pos;
		myTransform->SetPosition(pos);
	}

	void CameraScript::debugInputCamera()
	{
		Camera* cameraComponent = GetOwner()->GetComponent<Camera>();
		float viewSize = cameraComponent->GetSize();

		Transform* myTransform = GetOwner()->GetComponent<Transform>();
		Vector3 pos = myTransform->GetPosition();

		Camera::eProjectionType projectionType = cameraComponent->GetProjectionType();

		if (Input::GetKey(eKeyCode::NUM_9))
		{
			pos += myTransform->Forward() * 5.0f * (float)Time::DeltaTime();
			mOriginPosition = pos;
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_3))
		{
			pos += -myTransform->Forward() * 5.0f * (float)Time::DeltaTime();
			mOriginPosition = pos;
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_4))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += -myTransform->Right() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += -myTransform->Right() * -pos.z * (float)Time::DeltaTime();
			mOriginPosition = pos;
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_6))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += myTransform->Right() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += myTransform->Right() * -pos.z * (float)Time::DeltaTime();
			mOriginPosition = pos;
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_8))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += myTransform->Up() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += myTransform->Up() * -pos.z * (float)Time::DeltaTime();
			mOriginPosition = pos;
			myTransform->SetPosition(pos);
		}
		else if (Input::GetKey(eKeyCode::NUM_2))
		{
			if (Camera::eProjectionType::Orthographic == projectionType)
				pos += -myTransform->Up() * viewSize * 5.0f * (float)Time::DeltaTime();
			else
				pos += -myTransform->Up() * -pos.z * (float)Time::DeltaTime();
			mOriginPosition = pos;
			myTransform->SetPosition(pos);
		}
		// num 0 : ProjectionType Change
		else if (Input::GetKeyDown(eKeyCode::NUM_0))
		{
			Camera* mainCam = renderer::mainCamera;
			Camera* uiCamera = renderer::uiCamera;

			Camera::eProjectionType projectionType = mainCam->GetProjectionType();

			if (Camera::eProjectionType::Orthographic == projectionType)
			{
				mainCam->SetProjectionType(Camera::eProjectionType::Perspective);
				uiCamera->SetProjectionType(Camera::eProjectionType::Perspective);
			}
			else
			{
				mainCam->SetProjectionType(Camera::eProjectionType::Orthographic);
				uiCamera->SetProjectionType(Camera::eProjectionType::Orthographic);
			}
		}
		// num 7 - 1 : size Up-Down
		else if (Input::GetKey(eKeyCode::NUM_7))
		{
			viewSize -= 1.f * (float)Time::DeltaTime();
			if (0.001 <= viewSize)
				cameraComponent->SetSize(viewSize);
		}
		else if (Input::GetKey(eKeyCode::NUM_1))
		{
			viewSize += 1.f * (float)Time::DeltaTime();
			cameraComponent->SetSize(viewSize);
		}
		// num 5 : Debug On-Off
	}
	void CameraScript::CameraShake()
	{
		if (!mCameraShaking)
			return;

		Transform* cameraTr = GetOwner()->GetComponent<Transform>();
		
		if (mShakeAccumulateTime >= mShakeValidTime)
		{
			mShakeAccumulateTime = 0.0f;
			mCameraShaking = false;
			//cameraTr->SetPosition(mOriginPosition);
			GameDataManager::SetCameraMovaPosition(math::Vector2(mOriginPosition.x, mOriginPosition.y));
		}
		else
			mShakeAccumulateTime += (float)Time::DeltaTime();

		// X흔들림 크기 (누적된 시간)
		float oscillationX = cos(mShakeAccumulateTime * mOscillationPower);
		// Y흔들림 크기 (누적된 시간)
		float oscillationY = sin(mShakeAccumulateTime * mOscillationPower);

		// 씬 내에서의 카메라 위치
		math::Vector2 currentPosition = GameDataManager::GetCameraMovaPosition();
		// 흔들림 보정
		float oscillationRatio = 35.0f;
		Vector2 oscillationPosition = Vector2(
			currentPosition.x + oscillationX / oscillationRatio, currentPosition.y + oscillationY / oscillationRatio);

		//cameraTr->SetPosition(oscillationPosition);
		GameDataManager::SetCameraMovaPosition(oscillationPosition);
		cameraTr->SetPosition(oscillationPosition);
	}
}
