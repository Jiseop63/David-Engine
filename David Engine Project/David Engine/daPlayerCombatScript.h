#pragma once
#include "daCombatScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	// WeaponScript -> CombatScript �� ������ �� !!
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
		void EquipWeapon(); 							// ���� ��ü�� �κ��丮�� ���ؼ� ȣ���
		
		virtual void ToDoAttack() override;				// playerScript���� ȣ��
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