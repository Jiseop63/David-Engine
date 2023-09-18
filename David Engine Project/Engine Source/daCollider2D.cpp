#include "daCollider2D.h"
#include "daGameObject.h"
#include "daRenderer.h"
#include "daConstantBuffer.h"
#include "../David Engine/daPlayerScript.h"

namespace da
{
	UINT Collider2D::ColliderNumber = 0;

	Collider2D::Collider2D()
		: Component(enums::eComponentType::Collider)
		, mTransform(nullptr)
		, mColliderID(0)
		, mTotalPosition(math::Vector3::Zero)
		, mTotalScale(math::Vector3::One)

		, mColliderShape(enums::eColliderShape::Rect)
		, mDetectionType(eDetectionType::Default)
		, mColliderColor(math::Vector4::Zero)
		, mCenter(math::Vector2::Zero)
		, mSize(math::Vector2::One)
		
		, mFoot(false)
		, mBody(false)
		, mIsCollision(false)
		, mGrounded(false)
		, mGroundBuffer(0)
		, mPlatformCollision(false)
		, mWallCollision(eWallCollisionState::None)
		, mEnvRotate(0.0f)
		, mEnvRotateBuffer(0.0f)
		, mEnvPositionBuffer(math::Vector3::Zero)
	{
		mColliderID = ColliderNumber++;
		mColliderColor = math::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	Collider2D::~Collider2D()
	{
	}
	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();		
	}
	void Collider2D::Update()
	{
		if (!mUsing)
			return;
		// 뭐라도 하나 충돌중이면 Ground 유지하도록 하기
		if (0 < mGroundBuffer)
			mGrounded = true;
		else
			mGrounded = false;
	}
	void Collider2D::LateUpdate()
	{
		if (!mUsing)
			return;
		Transform* tr = GetOwner()->GetComponent<Transform>();

		math::Vector3 scale = tr->GetScale();
		if (enums::eColliderShape::Rect== mColliderShape)
		{
			scale.x *= mSize.x;
			scale.y *= mSize.y;
		}
		else if (enums::eColliderShape::Circle == mColliderShape)
		{
			scale.x *= mSize.x;
			scale.y *= mSize.y;
		}
		

		math::Vector3 pos = tr->GetPosition();
		pos.x += mCenter.x;
		pos.y += mCenter.y;

		graphics::DebugMesh mesh = {};
		mesh.Position = pos;
		mesh.Scale = scale;
		mesh.Rotation = tr->GetRotation();
		mesh.Type = mColliderShape;
		mesh.Color = mColliderColor;
		renderer::PushDebugMeshAttribute(mesh);

		mTotalPosition = pos;
		mTotalScale = scale;
	}

	void Collider2D::ChangeCollisionColor(bool isCollision)
	{
		if (isCollision)
			mColliderColor = math::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		else
			mColliderColor = math::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	void Collider2D::ChangeLandColor(bool isCollision)
	{
		if (isCollision)
			mColliderColor = math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		else
			mColliderColor = math::Vector4(1.0f, 0.0f, 1.0f, 1.0f);
	}

	void Collider2D::ChangeColliderColor(bool isCollision)
	{
		switch (mDetectionType)
		{
		case da::Collider2D::eDetectionType::Default:
		{
			if (isCollision)
				mColliderColor = math::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
			else
				mColliderColor = math::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		}
			break;
		case da::Collider2D::eDetectionType::Env:
		{
			if (isCollision)
				mColliderColor = math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			else
				mColliderColor = math::Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		}
			break;
		case da::Collider2D::eDetectionType::Sensor:
		{
			if (isCollision)
				mColliderColor = math::Vector4(0.20f, 0.20f, 0.20f, 1.0f);
			else
				mColliderColor = math::Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		}
			break;		
		break;
		default:
			break;
		}
	}

	void Collider2D::SetDetectionType(eDetectionType type)
	{
		mDetectionType = type;
		if (eDetectionType::Default == mDetectionType)
		{
			mColliderColor = math::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (eDetectionType::Env == mDetectionType)
		{
			mColliderColor = math::Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		}
		if (eDetectionType::Sensor == mDetectionType)
		{
			mColliderColor = math::Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		}
		if (eDetectionType::Inactive == mDetectionType)
		{
			mColliderColor = math::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		}
	}

	void Collider2D::groundCheck(Collider2D* other, bool isEnter)
	{
		if (!mFoot)
			return;
		if (eDetectionType::Env != other->GetDetectionType())
			return;

		// env 오브젝트를 이동할때 막히지 않도록 예외처리
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		PlayerScript* player = dynamic_cast<PlayerScript*>(scripts[0]);
		if (player)
		{
			if (player->IsPassingPlatform())
			{
				mGroundBuffer = 0;
				player->ApplyPassingPlatform(false);

				return;
			}
		}

		if (isEnter)
		{

			// 플렛폼 이동 예외처리
			if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
				mPlatformCollision = true;
			else
				mPlatformCollision = false;


		// enter에서 기울기를 적용할겨면, 여기서 버퍼값을 현재값으로 덮어씌우고,
		// exit에서 기울기를 적용할거면, 버퍼값을 따로둔다

		// envDataBuffer와 현재 envData를 비교
			Transform* envTransform = other->GetOwner()->GetTransform();
			math::Vector3 envPosition = envTransform->GetPosition();
			float envRotateZ = envTransform->GetRotation().z;

			// 최초 초기화
			if (math::Vector3::Zero == mEnvPositionBuffer)
			{
				mEnvPositionBuffer = envPosition;
				mEnvRotate = envRotateZ;
			}
			mEnvRotateBuffer = envRotateZ;

			// 1) 높이 비교
			// 비교대상이 이전보다 위에 있음 : 평지 -> 오르막, 오르막 -> 평지
			if (mEnvPositionBuffer.y == envPosition.y)
			{
				mEnvRotate = mEnvRotateBuffer;
			}
			else if (mEnvPositionBuffer.y < envPosition.y) // 이전값과 현재 값을 비교~
			{
				// 2) 기울기 비교
				if (0 != mEnvRotateBuffer)	// 대상이 경사로임
				// 평지 -> 오르막 : enter 순서대로 기울기 값을 적용
					mEnvRotate = mEnvRotateBuffer;
				// 오르막 -> 평지 : exit 이후, 버퍼에 적용된 값을 적용
				else
				{
					if (0 == mGroundBuffer)
						mEnvRotate = mEnvRotateBuffer;
				}
			}
			// 비교대상이 이전보다 아래에 있음 : 평지 -> 내리막, 내리막 -> 평지
			else
			{
				// 2) 기울기 비교
				if (0 == mEnvRotateBuffer) // 대상이 평지임
				// 내리막 -> 평지 : enter 순서대로 기울기 값을 적용
					mEnvRotate = mEnvRotateBuffer;
				// 평지 -> 내리막 : exit 이후, 버퍼에 적용된 값을 적용
				else
				{
					if (0 == mGroundBuffer)
						mEnvRotate = mEnvRotateBuffer;
				}
			}

			// 높이 비교가 끝났으면, 버퍼에 달아주기 (enter에서만)
			mEnvPositionBuffer = envPosition;
			
			// ground 적용
			mGroundBuffer++;
		}
		// exit
		else
		{
			// 두 값이 다르면 버퍼에 있는 값을 적용함
			if (mEnvRotateBuffer != mEnvRotate)
				mEnvRotate = mEnvRotateBuffer;

			// ground 적용
			if (0 < mGroundBuffer)
				mGroundBuffer--;
		}
		
	}
	void Collider2D::wallCollisionCheck(Collider2D* other, bool isEnter)
	{
		// 내가 body 가 아닌경우 충돌검사 안함
		if (!mBody)
			return;
		// 상대가 벽 타입이 아닌경우 충돌검사 안함
		if (eDetectionType::Env != other->GetDetectionType())
			return;

		// 상대가 플렛폼이면 충돌검사 안함
		if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
			return;


		if (!isEnter)
		{
			mWallCollision = eWallCollisionState::None;
			return;
		}
		// 충돌체 정보 가져오기
		math::Vector3 envPosition = other->GetTotalPosition();
		math::Vector3 envSize = other->GetTotalScale();
		math::Vector3 bodyPosition = GetTotalPosition();
		math::Vector3 bodySize = GetTotalScale();
		
		// 기준값 가져오기
		float wallTop = envPosition.y - (envSize.y / 2.0f);
		float bodyTop = bodyPosition.y + (bodySize.y / 2.0f);
		float wallLeft = envPosition.x + (envSize.x / 2.0f);
		float bodyLeft = bodyPosition.x - (bodySize.x / 2.0f);
		float wallRight = envPosition.x - (envSize.x / 2.0f);
		float bodyRight = bodyPosition.x + (bodySize.x / 2.0f);

		// 더 작은쪽이 충돌한부분임
		float left = abs(wallLeft) - abs(bodyLeft);
		float right = abs(wallRight) - abs(bodyRight);

		// 충돌 오차값
		float Xlength = (envSize.x / 2.0f) + (bodySize.x / 2.0f);
		float XPos = abs(envPosition.x) - abs(bodyPosition.x);

		// 두 값이 -인 경우
		if ((0 >= bodyPosition.x && 0 >= envPosition.x))
		{
			// 둘중 큰 값으로 계산
			if (bodyPosition.x <= envPosition.x)
				XPos = abs(bodyPosition.x) - abs(envPosition.x);
		}
		else if ((0 >= bodyPosition.x || 0 >= envPosition.x))
			XPos = abs(envPosition.x) + abs(bodyPosition.x);

		if (0 >= XPos)
			XPos = abs(XPos);

			
		float Ylength = (envSize.y / 2.0f) + (bodySize.y / 2.0f);
		float YPos = abs(envPosition.y) - abs(bodyPosition.y);

		// 두 값이 -인 경우
		if ((0 >= bodyPosition.y && 0 >= envPosition.y))
		{
			// 둘중 큰 값으로 계산
			if (bodyPosition.y <= envPosition.y)
				YPos = abs(bodyPosition.y) - abs(envPosition.y);
		}
		// 둘중 하나만 -인 경우
		else if ((0 >= bodyPosition.y || 0 >= envPosition.y))
			YPos = abs(envPosition.y) + abs(bodyPosition.y);

		if (0 >= YPos)
			YPos = abs(YPos);


		if ((Xlength - XPos <= 0.10f)
			&& (Xlength - XPos >= -0.10f))
		{
			if (abs(left) <= abs(right))
				mWallCollision = eWallCollisionState::Left;
			else
				mWallCollision = eWallCollisionState::Right;
		}
		else
			if ((Ylength - YPos <= 0.10f)
				&& (Ylength - YPos >= -0.10f))
				mWallCollision = eWallCollisionState::Top;
	}

	void Collider2D::OnCollisionEnter(Collider2D* other)
	{
		ChangeColliderColor(true);
		groundCheck(other, true);
		wallCollisionCheck(other, true);
		mIsCollision = true;

		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnCollisionEnter(other);
		}
	}
	void Collider2D::OnCollisionStay(Collider2D* other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnCollisionStay(other);
		}
	}
	void Collider2D::OnCollisionExit(Collider2D* other)
	{
		ChangeColliderColor(false);
		groundCheck(other, false);
		wallCollisionCheck(other, false);
		mIsCollision = false;

		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}
}