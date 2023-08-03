#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daLight.h"

#include "daWeaponScript.h"
#include "daEffectPlayerScript.h"

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
		void PlayerInput();
		void DebugInput();
		void UIInput();
		void GetMouse();
		void CalcPlayerDir();
		void ReverseTexture();
		void timeProcess();
		EffectPlayerScript* callEffect();
		void activeEffect(EffectPlayerScript* effect, const std::wstring name);
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
		WeaponScript* SetWeaponObject(GameObject* object);
		WeaponScript* GetWeaponScript() { return mWeaponScript; }
		EffectPlayerScript* AddEffectObject(GameObject* object);
		bool IsPlayerGround() { return mFootCollider->IsGround(); }
		void PlayerIsNotGround() { mFootCollider->ApplyGround(false); }
		math::Vector2 GetPlayerDir() { return mPlayerDir; }
		void SetPlayerPosition(math::Vector3 vector3) { mTransform->SetPosition(vector3); }
		void IsPlayerInDungeon(bool value) 
		{ 
			if (value)
				mLight->SetColor(math::Vector4(0.550f, 0.550f, 0.550f, 1.0f));
			else
				mLight->SetColor(math::Vector4::Zero);
		}
#pragma endregion
#pragma region Attack Logic
	private:
		// 행동 함수
		void InputAttack();
#pragma endregion
#pragma region Move Logic
		void InputMove();
		void walkDust();
#pragma endregion
#pragma region Jump & Dash Logic
	private:
		void jumpRegen();
		void dashRegen();

		void inputDash();
		void inputJump();
		void bufferedJump();
		void todoJump();
#pragma endregion
#pragma region Initialize Player
	public:
		void InitAnimation();
		void InitCollider();
#pragma endregion
#pragma region Collision Func
	public:
		virtual void OnCollisionEnter(Collider2D* other) override {}
		virtual void OnCollisionStay(Collider2D* other) override {}
		virtual void OnCollisionExit(Collider2D* other) override {}
		virtual void OnLandEnter(Collider2D* other) override;
		virtual void OnLandStay(Collider2D* other) override;
		virtual void OnLandExit(Collider2D* other) override {}
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
		Collider2D*		mRightCollider;
		Collider2D*		mLeftCollider;
#pragma endregion
#pragma region Other Scripts
	private:
		WeaponScript*	mWeaponScript;
		std::vector<EffectPlayerScript*> mEffects;
#pragma endregion
#pragma region Global Data
	private:
		structs::sPlayerStat*	mPlayerStat;
		structs::sJumpCount*	mJumpCount;
		structs::sDashCount* 	mDashCount;
		structs::sInventory*	mInventoryData;
#pragma endregion
#pragma region Condition value
	private:
		math::Vector2 mPlayerDir;
		ePlayerState	mActiveState;
		ePlayerState	mpreviousState;
		int				mMoveCondition;
		float			mDustAccumulateTime;
		bool			mDead;
#pragma endregion
	};
}