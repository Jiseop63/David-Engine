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
		// ������� �����
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Life];

		// ������ ä���
		renderer::LifeCB data;
		data.MaxHP = mMaxValue;
		data.CurHP = mCurValue;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}
}
