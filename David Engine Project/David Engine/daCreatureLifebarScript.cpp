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
		structs::sCreatureStat creatureStat = mCreatureScript->GetCreatureStat();

		// ������� �����
		graphics::ConstantBuffer* outCB
			= renderer::constantBuffer[(UINT)graphics::eCBType::Life];

		// ������ ä���
		renderer::LifeCB data;
		data.MaxHP = creatureStat.MaxHP;
		data.CurHP = creatureStat.CurHP;

		outCB->SetData(&data);
		outCB->Bind(graphics::eShaderStage::VS);
		outCB->Bind(graphics::eShaderStage::PS);
	}

}
