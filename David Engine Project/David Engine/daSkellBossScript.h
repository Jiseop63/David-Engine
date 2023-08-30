#pragma once
#include "daScript.h"

// Resources::Load<Texture>(L"BossSpriteSheet", L"..\\Resources\\Texture\\Scene_Dungeon2F\\SkellBossSpriteSheet.png");

namespace da
{
	class Animator;
	class PlayerScript;
	class SkellBossScript : public Script
	{

		enum class eBossPattern
		{
			Barrage,
			OneWayLaser,
			ThreeWayLayer,
			SwardRain,
		};
		enum class eBossState
		{
			Idle,
			Attack,
			Dead,
		};
	public:
		SkellBossScript();
		virtual ~SkellBossScript();

		virtual void Initialize() override;
		virtual void Update() override;

#pragma region FSM
	private:
		void ChangeState(eBossState state);
		void BossFSM();
		void SkellBossHandleIdle();
		void SkellBossHandleAttack();
		void SkellBossHandleDead();
#pragma endregion
	public:
		Transform* GetBossTransform() { return mBossTransform; }

	private:
		void attackCoolDown();
		
	private:
		void retIdle();

#pragma region AttackPettern
		// 빔 날리는 패턴				4초 동안 피해받지 않으면
		// 빔 세번 날리는 패턴		점프를 3초 동안 안하면
		// 탄막 날리는 패턴			점프를 6회 이상 하면
		// 칼 떨어뜨리는 패턴			대시를 6번 이상 사용하면
		// 가장먼저 조건 충족되는대로 패턴 고정


		// 패턴 구현하려면 Time값을 가지는 변수가 많이 필요할듯?
#pragma endregion



#pragma region Collision

#pragma endregion


	private:
		Transform*		mBossTransform;
		Collider2D*		mBossCollider;
		Animator*		mBossAnimator;
		PlayerScript*	mPlayerScript;

	private:
		eBossState		mBossActiveState;
		eBossPattern	mBossPattern;
		bool			mAttackReady;
		float			mAttackCoolDownDelayTime;
		float			mAttackCoolDownAccumulateTime;

		// 비주얼 용도로 손? 스크립트 (이펙트랑 같이 사용하면 될듯)

		// 파티클 스크립트
		// 이펙트 스크립트
		// 투사체 스크립트
	};
}