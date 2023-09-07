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
		bool IsLeft() { if (0 >= mCreatureDir.x) return true; else return false; }
		void ReverseTexture() { mCreatureRenderer->SetReverse(IsLeft()); }
#pragma endregion


#pragma region public Func
	public:
		Transform* GetCreatureTransform() { return mCreatureTransform; }
		
		virtual void AddEffectObject(GameObject* effectScript) {}
		structs::sCreatureStat GetCreatureStat() { return *mCreatureStat; }
#pragma endregion

#pragma region Creature 공용 함수
	protected:
		void VisualUpdate();		// 왼쪽 오른쪽 구분해서 반전시킴
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
		bool					mBodyCollision;
		bool					mFootCollision;
#pragma endregion

	};
}