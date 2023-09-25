#pragma once
#include "daCreatureScript.h"
#include "daGameObject.h"
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

	class PlayerScript : public CreatureScript
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();
				
#pragma region Default Func
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void AddActionUnit(GameObject* unit) override;
		PlayerCombatScript* AddCombatObject(GameObject* object);


		void GetDamage();
		void GetHeal();
#pragma endregion

#pragma region Input func
	private:
		void inputDebug();					// hp UP Down
		void inputUI();						// inventory
		void inputAttack();
		void inputCameraMove();
		void inputMove();
		void inputDash();
#pragma endregion

#pragma region Update func
	private:
		void updatePlayerConditionCheck();	// hp -> 0 dead
		void updatePlayerInput();			// move + attack Key
		void updatePlayerDir();				// mouseInput
		void updatePlayerFSM();
		void updateWalkEffect();
		void updateJumpRegen();
		void updateDashRegen();
		void updatePassPlatformCheck();
		void updateBufferedJump();
		void updateDashValidTime();
#pragma endregion



#pragma region FSM Func
	public:
		void ChangeState(ePlayerState state);
	private:
		void handleIdle();
		void handleMove();
		void handleJump();
		void handleDead();
#pragma endregion

#pragma region public Func
	public:
		PlayerCombatScript* SetWeaponObject(GameObject* object);				// 삭제 예정
		PlayerCombatScript* GetWeaponScript() { return mPlayerCombatScript; }
		void ActiveWeapon() { mPlayerCombatScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active); }

		const structs::sDashData* GetDashData() const { return &mDashData; }

		void downDashCount() { mDashData.CurDashCount--; }

		bool IsDashRunning() { return mDashRunning; }
		void IsPlayerInDungeon(bool value)
		{
			if (value)
				mLight->SetColor(math::Vector4(0.550f, 0.550f, 0.550f, 1.0f));
			else
				mLight->SetColor(math::Vector4::Zero);
		}

		bool IsPassingPlatform() { return mPassPlatform; }
		void ApplyPassingPlatform(bool value) { mPassPlatform = value; }
		bool IsJumping() { return mJumping; }
		void StartJump() { mJumping = true; }
#pragma endregion

#pragma region ToDo func
	private:
		void todoDustSpawn();
		void todoDash();
		void todoJump();

		void clearJumpBuffer()
		{
			mJumpData.BufferedJump = false;
			mJumpData.JumpForceRatio = 0.0f;
			mJumpData.JumpAccumulateTime = 0.0f;
		}
		bool useDash()
		{
			if (0 < mDashData.CurDashCount)
			{
				mDashData.CurDashCount -= 1;
				return true;
			}
			return false;
		}

#pragma endregion

#pragma region Initialize Player
	public:
		void initializeAnimation();
		void initializeCollider();
		void initializeData();
#pragma endregion

#pragma region Collision Func
	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override {}
		virtual void OnCollisionExit(Collider2D* other) override;
#pragma endregion

// value
#pragma region Components
	private:
		Light*				mLight;
		PlayerCombatScript*	mPlayerCombatScript;
#pragma endregion

#pragma region Global Data
	private:
		structs::sJumpData	mJumpData;
		structs::sDashData 	mDashData;
#pragma endregion

#pragma region Condition value
	private:
		ePlayerState	mActiveState;
		ePlayerState	mpreviousState;

		int				mMoveCondition;
		structs::sActionTimeValues mDustTime;
		structs::sActionTimeValues mDashTime;

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
