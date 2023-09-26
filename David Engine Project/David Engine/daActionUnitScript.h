#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daAnimator.h"

namespace da
{
	enum class UnitActionType
	{
		Duration,
		Range,
		None,
	};
	enum class UnitRenderType
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
		void SetUnitTypes(UnitRenderType actionType, UnitUsageType usageType, UnitActionType unitActionType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(UnitRenderType actionType, UnitActionType unitActionType, UnitUsageType usageType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(UnitActionType unitActionType, UnitRenderType actionType, UnitUsageType usageType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(UnitActionType unitActionType, UnitUsageType usageType, UnitRenderType actionType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(UnitUsageType usageType, UnitActionType unitActionType, UnitRenderType actionType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(UnitUsageType usageType, UnitRenderType actionType, UnitActionType unitActionType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetOwnerScript(CreatureScript* ownerScript) { mOwnerScript = ownerScript; }
		void SetUnitInfo(const structs::sActionUnitInfo unitInfo)  { mUnitInfo = unitInfo; }
		const structs::sActionUnitInfo GetUnitInfo() { return mUnitInfo; }
#pragma endregion

#pragma region Initialize transform
		void SetOffsetPosition(math::Vector3 offset) { mOffsetPosition = offset; }
		void SetMoveDirection(math::Vector3 direction) { mUnitDirection = direction; }
		void SetOverridePosition(math::Vector3 position) { mUnitBeginPosition = position; mActionUnitTransform->SetPosition(position); }
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

	protected:
		math::Vector3	mOffsetPosition;
		math::Vector3	mUnitBeginPosition;
		math::Vector3	mUnitDirection;

		structs::sActionUnitInfo mUnitInfo;
	protected:
		UnitRenderType	mUnitRenderType;
		UnitUsageType	mUnitUsageType;
		UnitActionType	mUnitActionType;
		std::wstring	mUnitAnimationName;
		bool			mUnitAnimationLoop;
	};
}