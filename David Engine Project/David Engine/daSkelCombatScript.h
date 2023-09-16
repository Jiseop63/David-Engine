#pragma once
#include "daCombatScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class Animator;
	class MonsterScript;
	class SkelCombatScript : public CombatScript
	{
	public:
		SkelCombatScript();
		virtual ~SkelCombatScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		virtual void AddProjectileObject(GameObject* object) override;

		virtual void ToDoAttack() override;
		virtual void attackProjectile() override;

	public:
		void attackFinished() { mAttackFinished = true; }
		
	public:
		virtual void OnCollisionEnter(Collider2D* other) override;

	private:
		bool			mAttackFinished;
		bool			mPlayerDamaged;
	};
}