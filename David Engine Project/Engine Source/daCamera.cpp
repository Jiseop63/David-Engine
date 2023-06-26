#include "daCamera.h"
#include "daGameObject.h"
#include "daTransform.h"
#include "daApplication.h"

extern da::Application application;
namespace da
{
	using namespace math;

	Matrix Camera::mView = Matrix::Identity;
	Matrix Camera::mProjection = Matrix::Identity;

	Camera::Camera()
		: Component(enums::eComponentType::Camera)
		, mProjectionType(eProjectionType::Perspective)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(1000.0f)
		, mSize(10.0f)
		, mTransform(nullptr)

	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}
	void Camera::Update()
	{
	}
	void Camera::LateUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix();
	}
	void Camera::Render()
	{
	}
	bool Camera::CreateViewMatrix()
	{
		Vector3 pos = mTransform->GetPosition();

		// View Translate Matrix
		mView = Matrix::Identity;
		mView *= Matrix::CreateTranslation(-pos);

		// View Rotation Matrix
		Vector3 up = mTransform->Up();
		Vector3 right = mTransform->Right();
		Vector3 foward = mTransform->Forward();

		Matrix viewRotate;
		viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
		viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
		viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;
		mView *= viewRotate;

		return true;
	}
	bool Camera::CreateProjectionMatrix()
	{
		RECT rect = {};
		GetClientRect(application.GetHwnd(), &rect);
		float width = float(rect.right - rect.left);
		float height = float(rect.bottom - rect.top);
		mAspectRatio = width / height;;



		if (mProjectionType == eProjectionType::Orthographic)
		{
			float OrthorGraphicRatio = mSize / 1000.0f;
			width *= OrthorGraphicRatio;
			height *= OrthorGraphicRatio;

			mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
		}
		else
		{
			mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
		}

		return true;
	}
}
