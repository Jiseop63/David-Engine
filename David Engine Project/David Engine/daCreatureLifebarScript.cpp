#include "daCreatureLifebarScript.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"
#include "daGameObject.h"

#include "daCreatureScript.h"

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
		if (mCreatureScript)
		{
			math::Vector3 creaturePos = mCreatureScript->GetCreatureTransform()->GetPosition();
			creaturePos.y += 0.750f;
			GetOwner()->GetTransform()->SetPosition(creaturePos);
		}
	}

}
