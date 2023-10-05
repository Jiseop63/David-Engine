#pragma once
#include "daCombatScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daItemScript.h"

namespace da
{
	class PlayerCombatScript : public CombatScript
	{
	public:
		PlayerCombatScript();
		virtual ~PlayerCombatScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void EquipWeapon(ItemScript* weaponItem); 		// ���� ��ü�� �κ��丮�� ���ؼ� ȣ���

		virtual void ToDoAttack() override;				// playerScript���� ȣ��
	private:
		virtual void updateWeaponRotate() override;
		void updateAttackCooldown();

	private:
		std::shared_ptr<Texture>	mWeaponTexture;
		ItemScript*					mWeaponItem;

		bool						mWeaponCooldownReady;
		structs::sActionTimeValues	mWeaponCooldownTime;
	};
}