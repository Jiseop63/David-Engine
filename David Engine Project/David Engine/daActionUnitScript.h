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
		void SetUnitTypes(enums::eUnitRenderType actionType, enums::eUnitUsageType usageType, enums::eUnitActionType unitActionType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitRenderType actionType, enums::eUnitActionType unitActionType, enums::eUnitUsageType usageType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitActionType unitActionType, enums::eUnitRenderType actionType, enums::eUnitUsageType usageType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitActionType unitActionType, enums::eUnitUsageType usageType, enums::eUnitRenderType actionType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitUsageType usageType, enums::eUnitActionType unitActionType, enums::eUnitRenderType actionType)
		{
			mUnitRenderType = actionType; mUnitUsageType = usageType; mUnitActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitUsageType usageType, enums::eUnitRenderType actionType, enums::eUnitActionType unitActionType)
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
		enums::eUnitRenderType	mUnitRenderType;
		enums::eUnitUsageType	mUnitUsageType;
		enums::eUnitActionType	mUnitActionType;
		std::wstring	mUnitAnimationName;
		bool			mUnitAnimationLoop;
	};
}