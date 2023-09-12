#include "daDashCountScript.h"

#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	DashCountScript::DashCountScript()
		: mMaxValue(0.0f)
		, mCurValue(0.0f)
	{
		int a = 0;
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
		data.MaxDashCount = mMaxValue;
		data.CurDashCount = mCurValue;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}