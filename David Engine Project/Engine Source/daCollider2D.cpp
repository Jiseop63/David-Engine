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
		, mColorType(enums::eColliderColor::Magenta)
		, mPosition(math::Vector2::Zero)
		, mCenter(math::Vector2::Zero)
		, mSize(math::Vector2::One)
	{
		mColliderID = ColliderNumber++;
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

		mPosition = pos;

		graphics::DebugMesh mesh = {};
		mesh.Position = pos;
		mesh.Scale = scale;
		mesh.Rotation = tr->GetRotation();
		mesh.Type = enums::eColliderShape::Rect;

		renderer::PushDebugMeshAttribute(mesh);
	}

	void Collider2D::BindConstantBuffer()
	{
		// 상수버퍼 만들기
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Collider];

		// 데이터 채우기
		renderer::ColliderCB data;

		data.ColliderColorType = (UINT)mColorType;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}


	void Collider2D::OnCollisionEnter(Collider2D* other)
	{
		mColorType = enums::eColliderColor::Red;
		BindConstantBuffer();
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
		mColorType = enums::eColliderColor::Green;
		BindConstantBuffer();
		const std::vector<Script*>& scripts
			= GetOwner()->GetScripts();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}
}