#include "daDashCountScript.h"

#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	DashCountScript::DashCountScript()
		: mMaxValue(nullptr)
		, mCurValue(nullptr)
	{
	}
	DashCountScript::~DashCountScript()
	{
	}
	void DashCountScript::LateUpdate()
	{
		
	}
	void DashCountScript::BindConstantBuffer()
	{
		// ������� �����
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Count];

		// ������ ä���
		renderer::CountCB data;
		data.MaxDashCount = (float)*mMaxValue;
		data.CurDashCount = (float)*mCurValue;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}