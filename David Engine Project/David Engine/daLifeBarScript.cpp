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
	void LifeBarScript::Initialize()
	{
	}
	void LifeBarScript::Update()
	{
	}
	void LifeBarScript::LateUpdate()
	{
		// value 가져오기
		structs::eCreatureStat playerlife = GameDataManager::GetPlayerStat();

		// 상수버퍼 만들기
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Bar];

		// 데이터 채우기
		renderer::BarCB data;
		data.MaxValue = playerlife.maxHP;
		data.CurrentValue = playerlife.curHP;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
	void LifeBarScript::Render()
	{
	}
}