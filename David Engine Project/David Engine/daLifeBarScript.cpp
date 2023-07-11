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
		// value ��������
		structs::eCreatureStat playerlife = GameDataManager::GetPlayerStat();

		// ������� �����
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Bar];

		// ������ ä���
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