#include "daCreatureLifebarScript.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"
#include "daGameObject.h"

#include "daCreatureScript.h"
#include "daFlyingCreatureScript.h"

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
		else if (mFlyingCreatureScript)
		{
			math::Vector3 creaturePos = mFlyingCreatureScript->GetCreatureTransform()->GetPosition();
			creaturePos.y += 0.750f;
			GetOwner()->GetTransform()->SetPosition(creaturePos);
		}
	}

}
