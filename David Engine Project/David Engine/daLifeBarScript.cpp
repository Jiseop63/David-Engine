#include "daLifeBarScript.h"

#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	LifeBarScript::LifeBarScript()
	{
	}
	LifeBarScript::~LifeBarScript()
	{
	}
	void LifeBarScript::BindConstantBuffer()
	{
		// value ��������
		structs::sPlayerStat playerlife = GameDataManager::GetPlayerStat();

		// ������� �����
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Life];

		// ������ ä���
		renderer::LifeCB data;
		data.MaxHP = playerlife.MaxHP;
		data.CurHP = playerlife.CurHP;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}