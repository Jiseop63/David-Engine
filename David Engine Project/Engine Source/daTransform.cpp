#include "daTransform.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	using namespace enums;
	using namespace math;
	using namespace graphics;

	Transform::Transform()
		: Component(eComponentType::Transform)
		, mPosition(Vector3::Zero)
		, mRotation(Vector3::Zero)
		, mScale(Vector3::One)
	{
	}
	Transform::~Transform()
	{
	}
	void Transform::Initialize()
	{
	}
	void Transform::Update()
	{
	}
	void Transform::LateUpdate()
	{
	}
	void Transform::Render()
	{
	}
	void Transform::BindConstantBuffer()
	{
		ConstantBuffer* cb = renderer::constantBuffer[(UINT)graphics::eCBType::Transform];
		Vector4 position(mPosition.x, mPosition.y, mPosition.z, 1.0f);
		cb->SetData(&position);
		cb->Bind(eShaderStage::VS);
	}
}
