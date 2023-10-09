#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daAnimator.h"

namespace da
{
	
	class CreatureScript;
	class ActionUnitScript : public Script
	{
	public:
		ActionUnitScript();
		virtual ~ActionUnitScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

#pragma region Initialize type
	public:
		void SetOwnerScript(CreatureScript* ownerScript) { mOwnerScript = ownerScript; }
		
		void SetUnitTypes(structs::sActionUnitTypes unitTypes) { mUnitTypes = unitTypes; }		
		void SetUnitInfo(const structs::sActionUnitStat unitInfo)  { mUnitInfo = unitInfo; }
		const structs::sActionUnitStat GetUnitInfo() { return mUnitInfo; }
#pragma endregion

#pragma region Initialize transform
		void SetUnitScale(math::Vector3 scale) { mUnitScale = scale; }
		void SetUnitColliderSize(math::Vector2 size) { mUnitColliderSize = size; }
		void SetUnitOffset(math::Vector3 offset) { mUnitOffset = offset; }
		void SetUnitDirection(math::Vector3 direction) { mUnitDirection = direction; }
		void SetUnitRotateAngle(float angle) { mUnitRotateAngle = angle; }
		void SetUnitOverridePosition(math::Vector3 position) { mUnitBeginPosition = position; mActionUnitTransform->SetPosition(position); }
#pragma endregion

#pragma region initialize visual
		void SetUnitTexture(const std::shared_ptr<class Texture> texture) { mActionUnitRenderer->ChangeMaterialTexture(texture); }
		void SetUnitReverse(bool value) { mActionUnitRenderer->SetReverse(value); }
#pragma endregion
		
	public:
		virtual void OnActive();

	public:
		virtual void ClearUnit();
#pragma region Collision func
	public:
		virtual void OnCollisionEnter(Collider2D* other);

		
#pragma endregion


	protected:
		Transform*		mActionUnitTransform;
		Collider2D*		mActionUnitCollider;
		MeshRenderer*	mActionUnitRenderer;
		Animator*		mActionUnitAnimator;

		CreatureScript* mOwnerScript;

	protected:
		math::Vector3	mUnitScale;
		math::Vector3	mUnitBeginPosition;
		math::Vector3	mUnitOffset;
		math::Vector3	mUnitDirection;
		math::Vector2	mUnitColliderSize;
		float			mUnitRotateAngle;

	protected:
		std::wstring				mUnitName;		// texture?
		// 투사체 속성
		structs::sActionUnitStat	mUnitInfo;
		structs::sActionUnitTypes	mUnitTypes;
	};
}