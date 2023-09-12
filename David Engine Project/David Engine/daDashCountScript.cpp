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
		// 상수버퍼 만들기
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Count];

		// 데이터 채우기
		renderer::CountCB data;
		data.MaxDashCount = (float)*mMaxValue;
		data.CurDashCount = (float)*mCurValue;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}