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
		// ���� �ϳ� �浹���̸� Ground �����ϵ��� �ϱ�
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

		// env ������Ʈ�� �̵��Ҷ� ������ �ʵ��� ����ó��
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

			// �÷��� �̵� ����ó��
			if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
				mPlatformCollision = true;
			else
				mPlatformCollision = false;


		// enter���� ���⸦ �����Ұܸ�, ���⼭ ���۰��� ���簪���� ������,
		// exit���� ���⸦ �����ҰŸ�, ���۰��� ���εд�

		// envDataBuffer�� ���� envData�� ��
			Transform* envTransform = other->GetOwner()->GetTransform();
			math::Vector3 envPosition = envTransform->GetPosition();
			float envRotateZ = envTransform->GetRotation().z;

			// ���� �ʱ�ȭ
			if (math::Vector3::Zero == mEnvPositionBuffer)
			{
				mEnvPositionBuffer = envPosition;
				mEnvRotate = envRotateZ;
			}
			mEnvRotateBuffer = envRotateZ;

			// 1) ���� ��
			// �񱳴���� �������� ���� ���� : ���� -> ������, ������ -> ����
			if (mEnvPositionBuffer.y == envPosition.y)
			{
				mEnvRotate = mEnvRotateBuffer;
			}
			else if (mEnvPositionBuffer.y < envPosition.y) // �������� ���� ���� ��~
			{
				// 2) ���� ��
				if (0 != mEnvRotateBuffer)	// ����� ������
				// ���� -> ������ : enter ������� ���� ���� ����
					mEnvRotate = mEnvRotateBuffer;
				// ������ -> ���� : exit ����, ���ۿ� ����� ���� ����
				else
				{
					if (0 == mGroundBuffer)
						mEnvRotate = mEnvRotateBuffer;
				}
			}
			// �񱳴���� �������� �Ʒ��� ���� : ���� -> ������, ������ -> ����
			else
			{
				// 2) ���� ��
				if (0 == mEnvRotateBuffer) // ����� ������
				// ������ -> ���� : enter ������� ���� ���� ����
					mEnvRotate = mEnvRotateBuffer;
				// ���� -> ������ : exit ����, ���ۿ� ����� ���� ����
				else
				{
					if (0 == mGroundBuffer)
						mEnvRotate = mEnvRotateBuffer;
				}
			}

			// ���� �񱳰� ��������, ���ۿ� �޾��ֱ� (enter������)
			mEnvPositionBuffer = envPosition;
			
			// ground ����
			mGroundBuffer++;
		}
		// exit
		else
		{
			// �� ���� �ٸ��� ���ۿ� �ִ� ���� ������
			if (mEnvRotateBuffer != mEnvRotate)
				mEnvRotate = mEnvRotateBuffer;

			// ground ����
			if (0 < mGroundBuffer)
				mGroundBuffer--;
		}
		
	}
	void Collider2D::wallCollisionCheck(Collider2D* other, bool isEnter)
	{
		// ���� body �� �ƴѰ�� �浹�˻� ����
		if (!mBody)
			return;
		// ��밡 �� Ÿ���� �ƴѰ�� �浹�˻� ����
		if (eDetectionType::Env != other->GetDetectionType())
			return;

		// ��밡 �÷����̸� �浹�˻� ����
		if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
			return;


		if (!isEnter)
		{
			mWallCollision = eWallCollisionState::None;
			return;
		}
		// �浹ü ���� ��������
		math::Vector3 envPosition = other->GetTotalPosition();
		math::Vector3 envSize = other->GetTotalScale();
		math::Vector3 bodyPosition = GetTotalPosition();
		math::Vector3 bodySize = GetTotalScale();
		
		// ���ذ� ��������
		float wallTop = envPosition.y - (envSize.y / 2.0f);
		float bodyTop = bodyPosition.y + (bodySize.y / 2.0f);
		float wallLeft = envPosition.x + (envSize.x / 2.0f);
		float bodyLeft = bodyPosition.x - (bodySize.x / 2.0f);
		float wallRight = envPosition.x - (envSize.x / 2.0f);
		float bodyRight = bodyPosition.x + (bodySize.x / 2.0f);

		// �� �������� �浹�Ѻκ���
		float left = abs(wallLeft) - abs(bodyLeft);
		float right = abs(wallRight) - abs(bodyRight);

		// �浹 ������
		float Xlength = (envSize.x / 2.0f) + (bodySize.x / 2.0f);
		float XPos = abs(envPosition.x) - abs(bodyPosition.x);

		// �� ���� -�� ���
		if ((0 >= bodyPosition.x && 0 >= envPosition.x))
		{
			// ���� ū ������ ���
			if (bodyPosition.x <= envPosition.x)
				XPos = abs(bodyPosition.x) - abs(envPosition.x);
		}
		else if ((0 >= bodyPosition.x || 0 >= envPosition.x))
			XPos = abs(envPosition.x) + abs(bodyPosition.x);

		if (0 >= XPos)
			XPos = abs(XPos);

			
		float Ylength = (envSize.y / 2.0f) + (bodySize.y / 2.0f);
		float YPos = abs(envPosition.y) - abs(bodyPosition.y);

		// �� ���� -�� ���
		if ((0 >= bodyPosition.y && 0 >= envPosition.y))
		{
			// ���� ū ������ ���
			if (bodyPosition.y <= envPosition.y)
				YPos = abs(bodyPosition.y) - abs(envPosition.y);
		}
		// ���� �ϳ��� -�� ���
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