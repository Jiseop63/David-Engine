#include "daCollider2D.h"
#include "daGameObject.h"
#include "daRenderer.h"
#include "daConstantBuffer.h"
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
		, mGrounded(false)
		, mWallCollision(eWallCollisionState::None)
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
	void Collider2D::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		math::Vector3 scale = tr->GetScale();
		scale.x *= mSize.x;
		scale.y *= mSize.y;

		math::Vector3 pos = tr->GetPosition();
		pos.x += mCenter.x;
		pos.y += mCenter.y;

		graphics::DebugMesh mesh = {};
		mesh.Position = pos;
		mesh.Scale = scale;
		mesh.Rotation = tr->GetRotation();
		mesh.Type = enums::eColliderShape::Rect;
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
	}

	void Collider2D::groundCheck(Collider2D* other, bool isEnter)
	{		
		if (!mFoot)
			return;

		if (isEnter)
			mGrounded = true;
		else
			mGrounded = false;
	}
	void Collider2D::wallCollisionCheck(Collider2D* other, bool isEnter)
	{
		// �ϴ� ���� BodyŸ������ Ȯ��
		if (!mBody)
			return;
		if (!isEnter)
		{
			mWallCollision = eWallCollisionState::None;
			return;
		}

		// ��벨
		math::Vector3 envPosition = other->GetTotalPosition();
		math::Vector3 envSize = other->GetTotalScale();

		// ����
		math::Vector3 bodyPosition = GetTotalPosition();
		math::Vector3 bodySize = GetTotalScale();
		
		// ScreenPos�� ��ȯ�ؼ� ���ؾ��ҵ�?


		float wallPosX = envPosition.x - (envSize.x / 2.0f);
		float bodyPosX = bodySize.x + (bodySize.x / 2.0f);
		float wallPosY = envPosition.y - (envSize.y / 2.0f);
		float bodyPosY = bodyPosition.y + (bodySize.y / 2.0f);

		// �������� Ȯ��
		if (envPosition.x <= bodySize.x)
		{
			// õ�忡 ��Ҵ��� Ȯ��
			if (abs(wallPosY) <= abs(bodyPosY))
				mWallCollision = eWallCollisionState::LT;
			else
				mWallCollision = eWallCollisionState::Left;
		}
		else if (envPosition.x >= bodySize.x)
		{
			if (abs(wallPosY) <= abs(bodyPosY))
				mWallCollision = eWallCollisionState::RT;
			else
				mWallCollision = eWallCollisionState::Right;
		}
		else
			mWallCollision = eWallCollisionState::Top;



		// ���� õ�忡 �Ӹ��� �ھ���
		//if (wallPosY <= bodyPosY)
		//{
		//	// �ٵ� ������ LT
		//	if (envPosition.x + (envSize.x / 2.0f)
		//		<= bodySize.x - (bodySize.x / 2.0f))
		//		mWallCollision = eWallCollisionState::LT;
		//	// �ƴ� ������ RT
		//	else if(envPosition.x - (envSize.x / 2.0f)
		//		>= bodySize.x + (bodySize.x / 2.0f))
		//		mWallCollision = eWallCollisionState::RT;
		//	else
		//		mWallCollision = eWallCollisionState::Top;
		//}
		//else
		//{
		//	// �׳� ����
		//	if (envPosition.x + (envSize.x / 2.0f)
		//		<= bodySize.x - (bodySize.x / 2.0f))
		//		mWallCollision = eWallCollisionState::Left;
		//	// �׳� ������
		//	else if (envPosition.x - (envSize.x / 2.0f)
		//		>= bodySize.x + (bodySize.x / 2.0f))
		//		mWallCollision = eWallCollisionState::Right;
		//}
	}

	void Collider2D::OnCollisionEnter(Collider2D* other)
	{
		ChangeColliderColor(true);
		groundCheck(other, true);
		wallCollisionCheck(other, true);

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

		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}
}