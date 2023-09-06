#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daLight.h"
#include "daPlayerCombatScript.h"

namespace da
{
	// 라디안 방향
#define RW		1.570f
#define RNW		0.785f
#define RN		0
#define RNE		-0.785f
#define RE		-1.570f
#define RSE		-2.335f
#define RS		-3.140f
#define RSW		-3.925f

	enum class ePlayerState
	{
		Idle,
		Move,
		Jump,
		Dead,
	};

	class EffectScript;
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();
		// 임시
	public:
		void GetDamage();
		void GetHeal();

#pragma region Default Func
		virtual void Initialize();
		virtual void Update();
#pragma endregion

#pragma region common Func
		void PlayerCondition(); // hp -> 0 dead
		void PlayerInput();		// move + attack Key
		void DebugInput();		// hp UP Down
		void UIInput();			// inventory
		void GetMouse();		// mouseInput
		void CalcPlayerDir();

		void timeProcess();
		EffectScript* callEffect();
		void activeEffect(EffectScript* effect, const std::wstring name);
#pragma endregion

#pragma region FSM Func
		void ChangeState(ePlayerState state);
		void PlayerFSM();
		void HandleIdle();
		void HandleMove();
		void HandleJump();
		void HandleDead();
#pragma endregion

#pragma region public Func
	public:
		CombatScript* SetWeaponObject(GameObject* object);
		CombatScript* GetWeaponScript() { return mWeaponScript; }
		EffectScript* AddEffectObject(GameObject* object);
		bool IsLeft() { if (0 >= mPlayerDir.x) return true; return false; }
		bool IsPlayerGround() { return mFootCollider->IsGround(); }
		void PlayerIsNotGround() { mFootCollider->ClearGroundBuffer(); }
		bool IsDashRunning() { return mDashRunning; }
		math::Vector2 GetPlayerDir() { return mPlayerDir; }
		void SetPlayerVelocity(math::Vector2 vector2) { mRigidbody->OverrideVelocity(vector2, 0.0f); }
		void SetPlayerPosition(math::Vector3 vector3) { mTransform->SetPosition(vector3); }
		void IsPlayerInDungeon(bool value)
		{ 
			if (value)
				mLight->SetColor(math::Vector4(0.550f, 0.550f, 0.550f, 1.0f));
			else
				mLight->SetColor(math::Vector4::Zero);
		}
#pragma endregion

#pragma region Weapon Logic
	private:
		// 행동 함수
		void InputAttack();
		void ChangeWeapon();
#pragma endregion

#pragma region Move Logic
	private:
		void CameraMove();
		void InputMove();
		void walkEffect();
#pragma endregion

#pragma region Effect
		void dustSpawn();
#pragma endregion


#pragma region Jump & Dash Logic
	private:
		void jumpRegen();
		void dashRegen();
		void endJumping();

		void bufferedJump();

		void inputDash();
		void todoDash();
		// 대시공격 추가해야함
		void inputJump();
		void todoJump();

	public:
		bool IsPassingPlatform() { return mPassPlatform; }
		void ApplyPassingPlatform(bool value) { mPassPlatform = value; }
		bool IsJumping() { return mJumping; }
		void StartJump() { mJumping = true; }
#pragma endregion
#pragma region Initialize Player
	public:
		void InitAnimation();
		void InitCollider();
#pragma endregion

#pragma region Collision Func
	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override {}
		virtual void OnCollisionExit(Collider2D* other) override;
#pragma endregion
// value
#pragma region Components
		// component val
	protected:
		Transform*		mTransform;
		Rigidbody*		mRigidbody;
		Animator*		mAnimator;
		MeshRenderer*	mRenderer;
		Light*			mLight;
		
		Collider2D*		mBodyCollider;
		Collider2D*		mFootCollider;
#pragma endregion

#pragma region Other Scripts
	private:
		CombatScript*	mWeaponScript;
		std::vector<EffectScript*> mEffects;
#pragma endregion

#pragma region Global Data
	private:
		structs::sPlayerStat*	mPlayerStat;
		structs::sJumpCount*	mJumpCount;
		structs::sDashCount* 	mDashCount;
#pragma endregion

#pragma region Condition value
	private:
		bool			mDead;
		math::Vector2	mPlayerDir;
		ePlayerState	mActiveState;
		ePlayerState	mpreviousState;

		int				mMoveCondition;
		float			mDustAccumulateTime;

		bool			mDashRunning;
		float			mHoldingDashTime;

		// 플렛폼 예외처리 변수
	private:
		bool			mJumping;
		bool			mPassPlatform;
		bool			mBodyCollision;
		bool			mFootCollision;
#pragma endregion
	};
}

/*
필요한 변수와 스크립트를 구분해서 역할 나누기

*/