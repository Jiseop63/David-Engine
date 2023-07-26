#include "guiDebugObject.h"
#include "daTransform.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace gui
{
	DebugObject::DebugObject()
	{
		AddComponent<da::Transform>();
	}

	DebugObject::~DebugObject()
	{
	}
	void DebugObject::BindColor(da::math::Vector4 color)
	{
		da::graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)da::graphics::eCBType::Collider];

		// 데이터 채우기
		renderer::ColliderCB data;

		data.ColliderColor = color;

		outCB->SetData(&data);
		outCB->Bind(da::graphics::eShaderStage::VS);
		outCB->Bind(da::graphics::eShaderStage::PS);
	}
}
