#include "daGridScript.h"
#include "daApplication.h"

#include "daRenderer.h"
#include "daConstantBuffer.h"

#include "daGameObject.h"
#include "daCamera.h"
#include "daTransform.h"

extern da::Application application;
namespace da
{
	GridScript::GridScript()
		: mCamera(nullptr)
	{
	}
	GridScript::~GridScript()
	{
	}

	void GridScript::Initialize()
	{
	}
	void GridScript::Update()
	{
	}
	void GridScript::LateUpdate()
	{
		if (nullptr == mCamera)
			return;

		// 준비물
		GameObject* gridObject = mCamera->GetOwner();
		Transform* gridTransform = gridObject->GetComponent<Transform>();

		// CameraPosition  가져오기
		math::Vector3 position = gridTransform->GetPosition();

		// CameraScale 가져오기
		float scale = mCamera->GetSize();

		// Resolution 가져오기
		float width = application.GetClientWidth();
		float height = application.GetClientHeight();
		math::Vector2 resolution(width, height);

		/// 상수버퍼 바인드
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Grid];

		renderer::GridCB data;
		data.CameraPosition = math::Vector4(position.x, position.y, position.z, 1.0f);
		data.CameraScale = math::Vector2(scale, scale);
		data.Resolution = resolution;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}

	void GridScript::Render()
	{
	}
}