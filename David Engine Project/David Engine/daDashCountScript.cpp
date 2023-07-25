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
		// value ��������
		structs::sDashCount dashCount = GameDataManager::GetDashCount();

		// ������� �����
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Count];

		// ������ ä���
		renderer::CountCB data;
		data.MaxCount = (float)dashCount.MaxCount;
		data.CurCount = (float)dashCount.CurCount;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}