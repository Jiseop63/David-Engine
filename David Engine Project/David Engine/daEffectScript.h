#pragma once
#include "daScript.h"
#include "daGameObject.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	class CombatScript;
	class EffectScript : public Script
	{
	public:
		enum eEffectMoveType
		{
			DontMove,
			Direction,
			Rotate,
		};
	public:
		EffectScript();
		virtual ~EffectScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();

	public:
		virtual void PlayEffect(const std::wstring name);
		virtual void OnActive();
			
		// 이펙트 이동방식
	protected:
		void stayEffect();		// 이동 안함
		void directionEffect();	// dir방향으로 이동
		void rotateEffect();	// 회전하여 up 방향으로 이동

	public:
		Transform* GetTransform() { return mEffectTransform; }
		
		void SetEffectPosition(math::Vector3 vector3) { mEffectTransform->SetPosition(vector3); }
		math::Vector3 GetEffectPosition() { return mEffectTransform->GetPosition(); }
		void SetEffectRotation(math::Vector3 vector3) { mEffectTransform->SetRotation(vector3); }
		math::Vector3 GetEffectRotation() { return mEffectTransform->GetRotation(); }
		void SetEffectScale(math::Vector3 vector3) { mEffectTransform->SetScale(vector3); }
		math::Vector3 GetEffectScale() { return mEffectTransform->GetScale(); }
		void SetCombatScript(CombatScript* weapon) { mCombatScript = weapon; }
		CombatScript* GetCombatScript() { return mCombatScript; }

		void SetReverse(bool value) { mReverse = value; }

		void SetEffectDir(math::Vector3 direction) { mEffectDir = direction; }
		void SetEffectMoveType(eEffectMoveType moveType) { mEffectMoveType = moveType; }
		void SetProjectileInfo(structs::sProjectileStat* projectileStat) { mProjectileInfo = projectileStat; }
	public:
		virtual void retInactive() { GetOwner()->SetObjectState(GameObject::eObjectState::Inactive); }

	protected:
		Transform*		mEffectTransform;
		MeshRenderer*	mEffectRenderer;
		Animator*		mEffectAnimator;

	protected:
		CombatScript*				mCombatScript;

		structs::sProjectileStat*	mProjectileInfo;
		math::Vector3				mBeginPosition;

		eEffectMoveType				mEffectMoveType;
		math::Vector3				mEffectDir;
	private:
		bool			mReverse;
	};
}