#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daRigidbody.h"
#include "daAnimator.h"

namespace da
{
	class EffectScript;
	class CreatureScript : public Script
	{
	public:
		CreatureScript();
		virtual ~CreatureScript();

		virtual void Initialize();
#pragma region common Func
	

	public:
		bool IsCreatureGround() { return mCreatureFootCollider->IsGround(); }
		void CreatureIsNotGround() { mCreatureFootCollider->ClearGroundBuffer(); }
		math::Vector2 GetCreatureDir() { return mCreatureDir; }

		void SetCreatureVelocity(math::Vector2 vector2) { mCreatureRigidbody->OverrideVelocity(vector2, 0.0f); }
		void SetCreaturePosition(math::Vector3 vector3) { mCreatureTransform->SetPosition(vector3); }
#pragma endregion


#pragma region public Func
	public:
		Transform* GetCreatureTransform() { return mCreatureTransform; }		
		structs::sCreatureStat GetCreatureStat() { return *mCreatureStat; }

		// ��ӹ޴� �� Ŭ�������� ���� �������ֱ�
		virtual void AddEffectObject(GameObject* effectScript) {}
	protected:
		EffectScript* callEffect();		// ����Ʈ �迭���� ������ ����Ʈ�� ��������
#pragma endregion

#pragma region Creature ���� �Լ�
	protected:
		bool isLeft() { if (0 >= mCreatureDir.x) return true; else return false; }
		void reverseTexture() { mCreatureRenderer->SetReverse(isLeft()); }
		void visualUpdate();														// ���� ������ �����ؼ� ������Ŵ
#pragma endregion



#pragma region Collision Func
	public:
		virtual void OnCollisionEnter(Collider2D* other);
		virtual void OnCollisionExit(Collider2D* other) {}
		void OnDamaged();
#pragma endregion





#pragma region Components
	protected:
		Transform*				mCreatureTransform;
		MeshRenderer*			mCreatureRenderer;
		Rigidbody*				mCreatureRigidbody;
		Animator*				mCreatureAnimator;
		Collider2D*				mCreatureBodyCollider;
		Collider2D*				mCreatureFootCollider;
#pragma endregion

#pragma region Other script
	protected:
		std::vector<EffectScript*> mEffects;

#pragma endregion

#pragma region Datas
	protected:
		structs::sCreatureStat*	mCreatureStat;				// ����
#pragma endregion

#pragma region condition
	protected:
		math::Vector2			mCreatureDir;
		bool					mIsDead;
		bool					mBodyCollision;	// ��� �Ⱦ��� �����
		bool					mFootCollision;	// ��� �Ⱦ��� �����
#pragma endregion

	};
}