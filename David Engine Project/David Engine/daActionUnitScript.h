#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daAnimator.h"

namespace da
{
	enum class UnitActionType
	{
		Stay,				// dont move
		JustRotate,			// dont move & rotate
		UsingDirection,		// move to dir
		UsingRotation,		// move to rotate
	};
	enum class UnitUsageType
	{
		Default,			// Animation & Collider
		OnlyTexture,		// Texture 
		OnlyAnimation,		// Animation
		OnlyCollider,		// Collider
	};
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
		void SetUnitTypes(UnitActionType actionType, UnitUsageType usageType) { mUnitActionType = actionType; mUnitUsageType = usageType; }
		void SetUnitTypes(UnitUsageType usageType, UnitActionType actionType) { mUnitActionType = actionType; mUnitUsageType = usageType; }
		void SetOwnerScript(CreatureScript* ownerScript) { mOwnerScript = ownerScript; }
		void SetUnitInfo(const structs::sActionUnitInfo unitInfo)  { mUnitInfo = unitInfo; }
		const structs::sActionUnitInfo GetUnitInfo() { return mUnitInfo; }
#pragma endregion

#pragma region Initialize transform
		void SetOffsetPosition(math::Vector3 offset) { mOffsetPosition = offset; }
		void SetMoveDirection(math::Vector3 direction) { mUnitDirection = direction; }
#pragma endregion

#pragma region initialize visual
		virtual void SetNextAnimation(const std::wstring name, bool loop = true) { mUnitAnimationName = name; mUnitAnimationLoop = loop; }
		void SetTexture(const std::shared_ptr<class Texture> texture) { mActionUnitRenderer->ChangeMaterialTexture(texture); }
		void SetReverse(bool value) { mActionUnitRenderer->SetReverse(value); }
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

	private:
		math::Vector3	mOffsetPosition;
		math::Vector3	mUnitBeginPosition;
		math::Vector3	mUnitDirection;

		structs::sActionUnitInfo mUnitInfo;
	private:
		UnitActionType	mUnitActionType;
		UnitUsageType	mUnitUsageType;
		std::wstring	mUnitAnimationName;
		bool			mUnitAnimationLoop;
	};
}