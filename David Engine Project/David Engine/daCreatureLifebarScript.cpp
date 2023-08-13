#include "daCreatureLifebarScript.h"
#include "daCreatureScript.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"
#include "daGameObject.h"

namespace da
{
	CreatureLifebarScript::CreatureLifebarScript()
		: mCreatureScript(nullptr)
	{
	}

	CreatureLifebarScript::~CreatureLifebarScript()
	{
	}

	void CreatureLifebarScript::Update()
	{
		math::Vector3 creaturePos = mCreatureScript->GetCreatureTransform()->GetPosition();
		creaturePos.y += 0.750f;
		GetOwner()->GetTransform()->SetPosition(creaturePos);
	}

	void CreatureLifebarScript::BindConstantBuffer()
	{
		// 상수버퍼 만들기
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Life];

		// 데이터 채우기
		renderer::LifeCB data;
		data.MaxHP = mMaxValue;
		data.CurHP = mCurValue;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}
