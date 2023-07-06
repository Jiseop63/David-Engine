#include "daTransform.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"
#include "daCamera.h"
namespace da
{
	using namespace enums;
	using namespace math;
	using namespace graphics;

	Transform::Transform()
		: Component(eComponentType::Transform)
		, mParent(nullptr)
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
		mWorld = Matrix::Identity;
		
		Matrix scaleMatrix;
		scaleMatrix = Matrix::CreateScale(mScale);

		Matrix rotateMatrix;
		rotateMatrix = Matrix::CreateRotationX(mRotation.x);
		rotateMatrix *= Matrix::CreateRotationY(mRotation.y);
		rotateMatrix *= Matrix::CreateRotationZ(mRotation.z);

		mUp = Vector3::TransformNormal(Vector3::Up, rotateMatrix);
		mForward = Vector3::TransformNormal(Vector3::Forward, rotateMatrix);
		mRight = Vector3::TransformNormal(Vector3::Right, rotateMatrix);

		Matrix positionMatrix;
		positionMatrix.Translation(mPosition);

		mWorld = scaleMatrix * rotateMatrix * positionMatrix;

		if (mParent)
		{
			mWorld *= mParent->mWorld;
		}

	}
	void Transform::Render()
	{
	}
	void Transform::BindConstantBuffer()
	{
		renderer::TransformCB transformCB = {};
		transformCB.World = mWorld;
		transformCB.View = Camera::GetViewMatrix();
		transformCB.Projection = Camera::GetProjectionMatrix();
				
		ConstantBuffer* bindTransform = renderer::constantBuffer[(UINT)graphics::eCBType::Transform];
		bindTransform->SetData(&transformCB);
		bindTransform->Bind(eShaderStage::VS);
	}
	Vector3 Transform::GetScreenPosition()
	{
		// viewport 가져오기
		Viewport viewport = GetViewport();

		// 스크린 변환할 행렬 가져오기
		Matrix projMatrix = Camera::GetProjectionMatrix();
		Matrix viewMatrix = Camera::GetViewMatrix();
		
		viewport.Project(mPosition / 3.84150f, projMatrix, viewMatrix, mWorld, mScreenPos);

		return mScreenPos;
	}
	Viewport Transform::GetViewport()
	{
		D3D11_VIEWPORT deviceViewport = graphics::GetDevice()->GetViewPort();
		Viewport viewport =
			Viewport(deviceViewport.TopLeftX, deviceViewport.TopLeftY
				, deviceViewport.Width, deviceViewport.Height, deviceViewport.MinDepth, deviceViewport.MaxDepth);
		return viewport;
	}
}
