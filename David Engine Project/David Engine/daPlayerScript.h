#pragma once
#include "daCreatureScript.h"
#include "daGameObject.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daLight.h"
#include "daPlayerCombatScript.h"
#include "daInventoryScript.h"
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

	class InventoryScript;
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

		

		void GetDamage();
		void GetHeal(float value);
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
		void updateDamageCap();
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
		virtual void OnDamaged(float damage) override;
		PlayerCombatScript* GetCombatScript() { return mPlayerCombatScript; }
		AudioSource* GetPlayerAudio() { return mPlayerAudio; }
		InventoryScript* GetInventoryScript() { return mInventoryScript; }
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

		void DungeonClearSound();

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
			mCreatureFootCollider->ClearGroundBuffer();
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

		void SetInventoryScript(InventoryScript* inventoryScript) { mInventoryScript = inventoryScript; }
		PlayerCombatScript* AddWeaponObject(GameObject* object);
		virtual void AddActionUnit(GameObject* unit) override;
		PlayerCombatScript* AddCombatObject(GameObject* object);

#pragma endregion
	public:
		virtual void ClearCreature() override
		{
			// 제어 초기화
			if (mCreatureFootCollider)
				mCreatureFootCollider->ClearGroundBuffer();
			ApplayControl();
			mCreatureRigidbody->OverrideVelocity(math::Vector2::Zero, 0.0f);
			// 상태 초기화
			GetOwner()->SetObjectState(GameObject::eObjectState::Active);
			ChangeState(ePlayerState::Idle);
			ActiveWeapon();
			mPlayerCombatScript->EquipWeapon(mInventoryScript->GetActiveItemScript());
		}
		

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
		AudioSource*		mPlayerAudio;

		PlayerCombatScript*	mPlayerCombatScript;
		InventoryScript*	mInventoryScript;
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

		structs::sActionTimeValues mDustDelayTime;
		structs::sActionTimeValues mAfterimageTime;

		structs::sActionTimeValues mDashTime;
		bool			mDashRunning;
		float			mHoldingDashTime;

		structs::sActionTimeValues mDamageDelayTime;
		bool			mhitted;
		// 플렛폼 예외처리 변수
	private:
		bool			mJumping;
		bool			mPassPlatform;
		bool			mBodyCollision;
		bool			mFootCollision;
#pragma endregion
	};
}
