#include "daSkellBossProjectileScript.h"
#include "daGameObject.h"
 
#include "daAnimator.h"
#include "daResources.h"
#include "daTime.h"

#include "daSkellBossScript.h"
#include "daCreatureScript.h"

namespace da
{
	SkellBossProjectileScript::SkellBossProjectileScript()
	{
	}
	SkellBossProjectileScript::~SkellBossProjectileScript()
	{
	}

	void SkellBossProjectileScript::Initialize()
	{
		ActionUnitScript::Initialize();
		
		//mProjectileTransform->SetScale(math::Vector3(1.50f, 1.50f, 1.0f));
		mActionUnitCollider->SetSize(math::Vector2(0.60f, 0.60f));

		// �ִϸ��̼� �ʱ�ȭ
		mActionUnitAnimator->Create(L"SkellBossProjectile", Resources::Find<Texture>(L"SkellBossProjectile"), math::Vector2::Zero, math::Vector2(50.0f, 50.0f), 10, math::Vector2::Zero, 0.10f, 80.0f);
	}

	void SkellBossProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{	
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// �ǰ� ȣ��
			creatureScript->OnDamaged(7.0f);

			this->ClearUnit();
		}
	}
}
