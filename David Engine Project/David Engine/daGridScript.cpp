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

	void GridScript::LateUpdate()
	{
		if (nullptr == mCamera)
			return;

		// �غ�
		GameObject* gridObject = mCamera->GetOwner();
		Transform* gridTransform = gridObject->GetComponent<Transform>();

		// CameraPosition  ��������
		math::Vector3 position = gridTransform->GetPosition();

		// CameraScale ��������
		float scale = mCamera->GetSize();

		// Resolution ��������
		float width = (float)application.GetClientWidth();
		float height = (float)application.GetClientHeight();
		math::Vector2 resolution(width, height);

		/// ������� ���ε�
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Grid];

		renderer::GridCB data;
		data.CameraPosition = math::Vector4(position.x, position.y, position.z, 1.0f);
		data.CameraScale = math::Vector2(scale, scale);
		data.Resolution = resolution;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
	}
}