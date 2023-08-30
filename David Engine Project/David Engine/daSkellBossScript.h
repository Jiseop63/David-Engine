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
		// �� ������ ����				4�� ���� ���ع��� ������
		// �� ���� ������ ����		������ 3�� ���� ���ϸ�
		// ź�� ������ ����			������ 6ȸ �̻� �ϸ�
		// Į ����߸��� ����			��ø� 6�� �̻� ����ϸ�
		// ������� ���� �����Ǵ´�� ���� ����


		// ���� �����Ϸ��� Time���� ������ ������ ���� �ʿ��ҵ�?
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

		// ���־� �뵵�� ��? ��ũ��Ʈ (����Ʈ�� ���� ����ϸ� �ɵ�)

		// ��ƼŬ ��ũ��Ʈ
		// ����Ʈ ��ũ��Ʈ
		// ����ü ��ũ��Ʈ
	};
}