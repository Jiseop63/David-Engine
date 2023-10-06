#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daAudioSource.h"

namespace da
{
	class ActionUnitScript;
	class CreatureScript : public Script
	{
	public:
		CreatureScript();
		virtual ~CreatureScript();

		virtual void Initialize();

#pragma region ActionUnit func
		virtual void AddActionUnit(GameObject* unit) {}
		ActionUnitScript* callActionUnit();
#pragma endregion

#pragma region Clear func
		void InactiveObejct();
		void ActiveObejct();
		void ClearVelocity() { mCreatureRigidbody->OverrideVelocity(math::Vector2::Zero, 0.0f); }
		void ClearGround() { mCreatureFootCollider->ClearGroundBuffer(); }

		void SetCreaturePosition(math::Vector3 vector3) { mCreatureTransform->SetPosition(vector3); }
#pragma endregion

#pragma region Ground func
		void NotUseGravity() { mCreatureRigidbody->ApplyComponentUsing(false); }
		void UseGravity() { mCreatureRigidbody->ApplyComponentUsing(); }
		bool IsGround() { return mCreatureFootCollider->IsGround(); }
#pragma endregion

#pragma region Get component & value
		Transform* GetCreatureTransform() { return mCreatureTransform; }
		AudioSource* GetCreatureAudio() { return mCreatureAudio; }
		const structs::sCreatureStat* GetCreatureStat() { return &mCreatureStat; }
		math::Vector2 GetCreatureDir() { return mCreatureDir; }
#pragma endregion

#pragma region Visual func
		bool isLeft() { if (0 >= mCreatureDir.x) return true; else return false; }
		void reverseTexture() { mCreatureRenderer->SetReverse(isLeft()); }
		void visualUpdate();														// ���� ������ �����ؼ� ������Ŵ
#pragma endregion

#pragma region Common creature func
		void moveLeft();
		void moveRight();
#pragma endregion

#pragma region Collision func
	public:
		virtual void OnCollisionEnter(Collider2D* other);
		virtual void OnCollisionExit(Collider2D* other) {}
		virtual void OnDamaged(float damage);
		void CallHitEffect(math::Vector3 position);
#pragma endregion



#pragma region common Func
	public:
		void ApplayControl(bool value = true) { mIsControl = value; }

		virtual void ClearCreature()
		{
			// �߷� �浹 �ʱ�ȭ
			if (mCreatureFootCollider)
				mCreatureFootCollider->ClearGroundBuffer();
			// �Է� �ʱ�ȭ
			ApplayControl();
			// �ӵ� �ʱ�ȭ
			mCreatureRigidbody->OverrideVelocity(math::Vector2::Zero, 0.0f);
		}

		void CreatureIsNotGround() { mCreatureFootCollider->ClearGroundBuffer(); }	// �Ⱦ� ����
		void SetCreatureVelocity(math::Vector2 vector2) { mCreatureRigidbody->OverrideVelocity(vector2, 0.0f); }	// �Ⱦ� ����
#pragma endregion


#pragma region Components
	protected:
		Transform*				mCreatureTransform;
		MeshRenderer*			mCreatureRenderer;
		Rigidbody*				mCreatureRigidbody;
		Animator*				mCreatureAnimator;
		Collider2D*				mCreatureBodyCollider;
		Collider2D*				mCreatureFootCollider;
		AudioSource*			mCreatureAudio;
#pragma endregion

#pragma region Other script
	protected:
		std::vector<ActionUnitScript*> mActionUnits;

#pragma endregion

#pragma region Datas
	protected:
		structs::sCreatureStat	mCreatureStat;				// ����
		math::Vector2			mCreatureDir;
#pragma endregion

#pragma region condition
	protected:
		bool					mIsControl;
		float					mHitEffectAngle;
		bool					mIsDead;


		// foot, body collision check ������ �÷��� ����ó���ϴ� �뵵�� ���� ����
		bool					mBodyCollision;
		bool					mFootCollision;
#pragma endregion

	};
}