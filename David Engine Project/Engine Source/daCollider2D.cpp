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
		, mColliderShape(enums::eColliderShape::Rect)
		, mDetectionType(eColliderDetection::Default)
		, mCenter(math::Vector2::Zero)
		, mSize(math::Vector2::One)
		, mColliderColor(math::Vector4::Zero)
		, mGrounded(false)
		, mWallCollision(eWallCollisionState::None)
		, mTotalPosition(math::Vector3::Zero)
		, mTotalScale(math::Vector3::One)
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

	void Collider2D::SetColliderDetection(eColliderDetection type)
	{
		mDetectionType = type;
		if (eColliderDetection::Land == mDetectionType)
		{
			mColliderColor = math::Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		}
		if (eColliderDetection::Default == mDetectionType)
		{
			mColliderColor = math::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (eColliderDetection::Inactive == mDetectionType)
		{
			mColliderColor = math::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		}
	}

	void Collider2D::WallCollitionCkeck(Transform* wallTransform)
	{
		// 위치 비교하기
		if (GetOwner()->GetTransform()->GetPosition().x >= wallTransform->GetPosition().x)
		{
			mWallCollision = eWallCollisionState::Right;
		}
		else
			mWallCollision = eWallCollisionState::Left;

	}

	void Collider2D::OnCollisionEnter(Collider2D* other)
	{
		ChangeCollisionColor(true);
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
		ChangeCollisionColor(false);
		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}


	void Collider2D::OnGroundEnter(Collider2D* other)
	{
		if (enums::eLayerType::Land == other->GetOwner()->GetLayerType()
			&& enums::eLayerType::Land != GetOwner()->GetLayerType())			
			mGrounded = true;

		ChangeLandColor(true);
		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnGroundEnter(other);
		}
	}
	void Collider2D::OnGroundStay(Collider2D* other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnGroundStay(other);
		}
	}
	void Collider2D::OnGroundExit(Collider2D* other)
	{
		if (enums::eLayerType::Land == other->GetOwner()->GetLayerType()
			&& enums::eLayerType::Land != GetOwner()->GetLayerType())
			mGrounded = false;
		ChangeLandColor(false);
		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnGroundExit(other);
		}
	}
	void Collider2D::OnWallEnter(Collider2D* other)
	{
		if (enums::eLayerType::Land == other->GetOwner()->GetLayerType()
			&& enums::eLayerType::Land != GetOwner()->GetLayerType())
		{
			// 충돌 반별하기
			WallCollitionCkeck(other->GetOwner()->GetTransform());
		}

		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnWallEnter(other);
		}
	}
	void Collider2D::OnWallStay(Collider2D* other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnWallStay(other);
		}
	}
	void Collider2D::OnWallExit(Collider2D* other)
	{
		if (enums::eLayerType::Land == other->GetOwner()->GetLayerType()
			&& enums::eLayerType::Land != GetOwner()->GetLayerType())
		{
			mWallCollision = eWallCollisionState::None;
		}

		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnWallExit(other);
		}
	}
}