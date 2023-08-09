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
		// value 가져오기
		structs::sPlayerStat playerlife = GameDataManager::GetPlayerStat();

		// 상수버퍼 만들기
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Life];

		// 데이터 채우기
		renderer::LifeCB data;
		data.MaxHP = playerlife.MaxHP;
		data.CurHP = playerlife.CurHP;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}