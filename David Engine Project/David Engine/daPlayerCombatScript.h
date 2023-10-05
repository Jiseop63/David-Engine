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
		void EquipWeapon(ItemScript* weaponItem); 		// 무기 교체시 인벤토리를 통해서 호출됨

		virtual void ToDoAttack() override;				// playerScript에서 호출
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