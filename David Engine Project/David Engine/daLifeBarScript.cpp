#include "daLifeBarScript.h"

#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	LifeBarScript::LifeBarScript()
		: mMaxValue(nullptr)
		, mCurValue(nullptr)
	{
	}
	LifeBarScript::~LifeBarScript()
	{
	}
	void LifeBarScript::BindConstantBuffer()
	{
		// ������� �����
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Life];

		// ������ ä���
		renderer::LifeCB data;
		data.MaxHP = *mMaxValue;
		data.CurHP = *mCurValue;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}