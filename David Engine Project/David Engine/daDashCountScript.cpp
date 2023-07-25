#include "daDashCountScript.h"

#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	DashCountScript::DashCountScript()
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
		// value 가져오기
		structs::sDashCount dashCount = GameDataManager::GetDashCount();

		// 상수버퍼 만들기
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Count];

		// 데이터 채우기
		renderer::CountCB data;
		data.MaxCount = (float)dashCount.MaxCount;
		data.CurCount = (float)dashCount.CurCount;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}