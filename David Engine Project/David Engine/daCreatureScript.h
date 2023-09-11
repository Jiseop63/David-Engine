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

		// 상속받는 각 클래스마다 직접 구현해주기
		virtual void AddEffectObject(GameObject* effectScript) {}
	protected:
		EffectScript* callEffect();		// 이펙트 배열에서 유요한 이펙트를 가져오기
#pragma endregion

#pragma region Creature 공용 함수
	protected:
		bool isLeft() { if (0 >= mCreatureDir.x) return true; else return false; }
		void reverseTexture() { mCreatureRenderer->SetReverse(isLeft()); }
		void visualUpdate();														// 왼쪽 오른쪽 구분해서 반전시킴
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
		structs::sCreatureStat*	mCreatureStat;				// 공용
#pragma endregion

#pragma region condition
	protected:
		math::Vector2			mCreatureDir;
		bool					mIsDead;
		bool					mBodyCollision;	// 사실 안쓰는 기능임
		bool					mFootCollision;	// 사실 안쓰는 기능임
#pragma endregion

	};
}