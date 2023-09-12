#pragma once
#include "daCreatureScript.h"
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


		virtual void AddEffectObject(GameObject* effectObject) override;
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
		void inputJump();
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
		CombatScript* SetWeaponObject(GameObject* object);
		CombatScript* GetWeaponScript() { return mWeaponScript; }
		structs::sDashData GetDashData() const { return mDashData; }

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
		void todoActiveEffect(EffectScript* effect, const std::wstring name);
		void todoDustSpawn();
		void todoDash();
		void todoJump();
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
		Light*			mLight;
		CombatScript*	mWeaponScript;
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