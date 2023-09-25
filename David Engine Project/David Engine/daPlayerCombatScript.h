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
		void EquipWeapon(); 							// ���� ��ü�� �κ��丮�� ���ؼ� ȣ���		
		virtual void ToDoAttack() override;				// playerScript���� ȣ��
	private:
		virtual void updateWeaponRotate() override;

	private:
		std::shared_ptr<Texture>			mWeaponTexture;

		std::shared_ptr<Texture>			mFirstTexture;
		std::shared_ptr<Texture>			mSecondTexture;
	};
}