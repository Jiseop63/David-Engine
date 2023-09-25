#pragma once
#include "daCombatScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	class PlayerScript;
	class PlayerCombatScript : public CombatScript
	{
	public:
		PlayerCombatScript();
		virtual ~PlayerCombatScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void EquipWeapon(); 							// 무기 교체시 인벤토리를 통해서 호출됨		
		virtual void ToDoAttack() override;				// playerScript에서 호출
	private:
		virtual void updateWeaponRotate() override;

	private:
		std::shared_ptr<Texture>			mWeaponTexture;

		std::shared_ptr<Texture>			mFirstTexture;
		std::shared_ptr<Texture>			mSecondTexture;
	};
}