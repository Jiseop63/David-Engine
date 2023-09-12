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

}
