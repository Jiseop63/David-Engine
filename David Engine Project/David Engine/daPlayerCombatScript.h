#pragma once
#include "daCombatScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	// WeaponScript -> CombatScript 로 변경할 것 !!
	class PlayerScript;
	class lProjectileScript;
	class EffectScript;
	class PlayerCombatScript : public CombatScript
	{
	public:
		PlayerCombatScript();
		virtual ~PlayerCombatScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

#pragma region Initialize & Get
	public:		
		virtual void AddEffectObject(GameObject* object) override;
		virtual void AddProjectileObject(GameObject* object) override;

#pragma endregion

#pragma region call by player
	public:
		void EquipWeapon(); 							// 무기 교체시 인벤토리를 통해서 호출됨
		
		virtual void ToDoAttack() override;				// playerScript에서 호출
#pragma endregion

#pragma region update Transform & Render & Condition
	private:
		virtual void updateWeaponRotate() override;
#pragma endregion

	private:
		virtual void attackProjectile() override;

	private:
		std::shared_ptr<Texture>			mWeaponTexture;

		std::shared_ptr<Texture>			mFirstTexture;
		std::shared_ptr<Texture>			mSecondTexture;
	};
}