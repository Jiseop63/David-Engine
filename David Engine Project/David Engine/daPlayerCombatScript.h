#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	// WeaponScript -> CombatScript 로 변경할 것 !!
	class PlayerScript;
	class ProjectileScript;
	class EffectScript;
	class CombatScript : public Script
	{
	public:
		CombatScript();
		virtual ~CombatScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

#pragma region Initialize & Get
	public:
		void SetPlayerScript(PlayerScript* player) { mPlayerScript = player; }
		PlayerScript* GetPlayerScript() { return mPlayerScript; }
		
		void AddEffectObject(GameObject* object);
		void AddProjectileObject(GameObject* object);

	private:
		EffectScript* callEffect();
		ProjectileScript* callProjectile();
#pragma endregion

#pragma region call by player
	public:
		void EquipWeapon(); 							// 무기 교체시 인벤토리를 통해서 호출됨
		
		void ToDoAttack();								// playerScript에서 호출
		void CallHitEffect(math::Vector3 position);		// 피격 대상이 호출함
#pragma endregion

#pragma region update Transform & Render & Condition
	private:
		// 렌더링 방향 구하기
		bool IsLeft() { if (0 >= mPlayerDir.x) return true; else return false; }	// 방향을 어떻게 받지?

		// update 에서 Transform 및 render 세팅
		void updateWeaponPosition();			// Update에서 항상 플레이어 위치로 이동
		void updateWeaponRotate();				// Update에서 항상 마우스 방향으로 회전함
		void updateReverseRenderer() { bool value = IsLeft(); mWeaponRenderer->SetReverse(value); mWeaponRenderer->SetReverse(value); }

		void updateAttackCoolDown(); // 공격 스텟 정보로 쿨다운 적용하기
#pragma endregion

#pragma region Attack func
	private:
		// 공격 애니메이션 실행	*근거리는 bool 조건 하나를 두고 이미지를 교체, 원거리는 애니메이션 재생
		void playWeaponAttack();

		// 공격 스텟 정보로 이펙트 및 투사체 호출
		void attackEffect();			// 공격 이펙트 세팅		
		void attackProjectile();		// 공격 투사체 세팅
		void attackAnimation();			// 무기 이미지 변경해주는 함수

#pragma endregion

	protected:
		Transform*							mWeaponTransform;
		Animator*							mWeaponAnimator;
		MeshRenderer*						mWeaponRenderer;
		PlayerScript*						mPlayerScript;
		std::vector<EffectScript*>			mEffects;
		std::vector<ProjectileScript*>		mProjectiles;

	protected:
		structs::sArmour*					mActiveArmour;
		std::shared_ptr<Texture>			mWeaponTexture;

		std::shared_ptr<Texture>			mFirstTexture;
		std::shared_ptr<Texture>			mSecondTexture;

	private:
		std::wstring mEffectName;


	private:
		math::Vector3						mEffectScale;
		math::Vector2						mProjectileScale;
		math::Vector3						mPlayerPosition;
		math::Vector2						mPlayerDir;
		float								mHitEffectAngle;
		bool								mWeaponAttacked;

	private:

		// 개선의 여지가 있음
		float	mEffectAngle;
	};
}