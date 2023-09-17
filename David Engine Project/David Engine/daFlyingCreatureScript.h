#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daRigidbody.h"
#include "daAnimator.h"

namespace da
{
	class EffectScript;
	class FlyingCreatureScript : public Script
	{
	public:
		FlyingCreatureScript();
		virtual ~FlyingCreatureScript();

		virtual void Initialize();
#pragma region common Func

	public:
		math::Vector2 GetCreatureDir() { return mCreatureDir; }

		void SetCreatureVelocity(math::Vector2 vector2) { mCreatureRigidbody->OverrideVelocity(vector2, 0.0f); }
		void SetCreaturePosition(math::Vector3 vector3) { mCreatureTransform->SetPosition(vector3); }
#pragma endregion


#pragma region public Func
	public:
		Transform* GetCreatureTransform() { return mCreatureTransform; }
		const structs::sCreatureStat* GetCreatureStat() { return &mCreatureStat; }
		// ��ӹ޴� �� Ŭ�������� ���� �������ֱ�
		virtual void AddEffectObject(GameObject* effectScript) {}
	protected:
		EffectScript* callEffect();		// ����Ʈ �迭���� ������ ����Ʈ�� ��������
#pragma endregion

#pragma region Monster ���� �Լ�
	protected:
		bool isLeft() { if (0 >= mCreatureDir.x) return true; else return false; }
		void reverseTexture() { mCreatureRenderer->SetReverse(isLeft()); }
		void visualUpdate();														// ���� ������ �����ؼ� ������Ŵ

		virtual void moveLeft();
		virtual void moveRight();
#pragma endregion



#pragma region Collision Func
	public:
		virtual void OnCollisionEnter(Collider2D* other) {}
		virtual void OnCollisionExit(Collider2D* other) {}
		void OnDamaged(float damage);
#pragma endregion





#pragma region Components
	protected:
		Transform*		mCreatureTransform;
		MeshRenderer*	mCreatureRenderer;
		Rigidbody*		mCreatureRigidbody;
		Animator*		mCreatureAnimator;
		Collider2D*		mCreatureBodyCollider;
#pragma endregion

#pragma region Other script
	protected:
		std::vector<EffectScript*> mEffects;

#pragma endregion

#pragma region Datas
	protected:
		structs::sCreatureStat	mCreatureStat;				// ����
#pragma endregion

#pragma region condition
	protected:
		math::Vector2			mCreatureDir;
		bool					mIsDead;
#pragma endregion

	};
}