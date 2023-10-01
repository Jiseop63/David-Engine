#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class ItemScript : public Script
	{
	public:
		ItemScript();
		virtual ~ItemScript();

#pragma region UnitType
	public:
		void SetUnitTypes(enums::eUnitRenderType actionType, enums::eUnitUsageType usageType, enums::eUnitActionType unitActionType)
		{
			mUnitTypes.RenderType = actionType; mUnitTypes.UsageType = usageType; mUnitTypes.ActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitRenderType actionType, enums::eUnitActionType unitActionType, enums::eUnitUsageType usageType)
		{
			mUnitTypes.RenderType = actionType; mUnitTypes.UsageType = usageType; mUnitTypes.ActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitActionType unitActionType, enums::eUnitRenderType actionType, enums::eUnitUsageType usageType)
		{
			mUnitTypes.RenderType = actionType; mUnitTypes.UsageType = usageType; mUnitTypes.ActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitActionType unitActionType, enums::eUnitUsageType usageType, enums::eUnitRenderType actionType)
		{
			mUnitTypes.RenderType = actionType; mUnitTypes.UsageType = usageType; mUnitTypes.ActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitUsageType usageType, enums::eUnitActionType unitActionType, enums::eUnitRenderType actionType)
		{
			mUnitTypes.RenderType = actionType; mUnitTypes.UsageType = usageType; mUnitTypes.ActionType = unitActionType;
		}
		void SetUnitTypes(enums::eUnitUsageType usageType, enums::eUnitRenderType actionType, enums::eUnitActionType unitActionType)
		{
			mUnitTypes.RenderType = actionType; mUnitTypes.UsageType = usageType; mUnitTypes.ActionType = unitActionType;
		}
		const structs::sUnitTypes GetUnitTypes() { return mUnitTypes; }
#pragma endregion
	public:
		void SetUnitInfo(const structs::sActionUnitInfo unitInfo) { mUnitInfo = unitInfo; }
		const structs::sActionUnitInfo GetUnitInfo() { return mUnitInfo; }
		void SetUnitAnimation(const std::wstring name, bool loop = true) { mUnitAnimationInfo.Name = name; mUnitAnimationInfo.Loop = loop; }
		const structs::sAnimationInfo GetAnimationInfo() { return mUnitAnimationInfo; }

	protected:
		Transform*					mItemTransform;
		MeshRenderer*				mItemRenderer;


	protected:
		std::wstring				mItemName;
		// 투사체 속성
		structs::sUnitTypes			mUnitTypes;
		structs::sActionUnitInfo	mUnitInfo;
		structs::sAnimationInfo		mUnitAnimationInfo;
		// 공격 속성
		structs::sAttackStat		mAttackStat;
	};
}