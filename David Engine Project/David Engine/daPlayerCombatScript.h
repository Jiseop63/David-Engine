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
		// GameData 에 의해 공격 스텟 세팅해주기 (최초, 무기 교체시 호출) ? 무기 교체는 누가 호출해주나? GameDataManager? Player? 
		void EquipWeapon() {}					// gameData 에서 현재 장착중인 무기 enum 값을 기준으로 세팅됨
		// 플레이어가 공격할때 호출
		//void DoAttack() {}
#pragma endregion

#pragma region update Transform & Render & Condition
	private:
		// 렌더링 방향 구하기
		bool IsLeft() { /*if (0 >= mCreatureDir.x) return true; else return false;*/ }

		// update 에서 Transform 및 render 세팅
		void updateWeaponPosition() {}			// Update에서 항상 플레이어 위치로 이동
		void updateWeaponRotate() {}			// Update에서 항상 마우스 방향으로 회전함
		void updateReverseRenderer() { /*bool value = IsLeft(); mCreatureRenderer->SetReverse(value); mCreatureWeaponScript->SetReverse(value);*/ }

		// 공격 스텟 정보로 쿨다운 적용하기
		void updateAttackCoolDown() {};
#pragma endregion

#pragma region Attack func
	private:
		// 공격 애니메이션 실행	*근거리는 bool 조건 하나를 두고 이미지를 교체, 원거리는 애니메이션 재생
		void playWeaponAttack() {}

		// 공격 스텟 정보로 이펙트 및 투사체 호출
		void attackEffect() {}			// 공격 이펙트 세팅		
		void attackProjectile() {}		// 공격 투사체 세팅
#pragma endregion

#pragma region Weapon Func
	public:
		void CallHitEffect(math::Vector3 position);										// 타격 이펙트 호출을 위한 함수
		void SetWeaponTransform(math::Vector3 playerPos, math::Vector2 playerDir);		// 무기 위치와 방향을 세팅함 (복잡)
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }				// 이미지 반전 함수
		void ChangeWeapon();															// *안씀

		void DoAttack();																// 플레이어가 공격할때 호출하는 함수

	public:
		void ActiveEffect();															// 공격시 이펙트 호출
		void ActiveProjectile();														// 공격시 투사체 호출

	private:
		void attackConditionCheck();													// 아마 쿨다운 함수인듯
		void projectileConditionCheck();												// 아마 투사체 유지시간인듯
		void activeAttack();															// 안씀

		
		void weaponInit();																// 무기정보 세팅하는 함수
		void playWeaponImage();															// 무기 이미지 변경해주는 함수, 공격할때마다 호출
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