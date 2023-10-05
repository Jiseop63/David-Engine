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
		
		virtual void Initialize() override;

	public:
		void SetItemName(const std::wstring name) { mItemName = name; }
		const std::wstring GetItemName() { return mItemName; }
		void SetItemRenderType(enums::eItemRenderType itemRenderType) { mItemRenderType = itemRenderType; }
		const enums::eItemRenderType GetItemRenderType() { return mItemRenderType; }
		void SetItemTexture(std::shared_ptr<graphics::Texture> texture) { mItemRenderer->GetMaterial()->SetTexture(texture); }
		std::shared_ptr<graphics::Texture> GetItemTexture() { return mItemRenderer->GetMaterial()->GetTexture(); }
		void SetItemAnimationInfo(structs::sAnimationInfo animationInfo) { mItemAnimationInfo = animationInfo; }
		structs::sAnimationInfo GetItemAnimationInfo() { return mItemAnimationInfo; }
		void SetItemAttackStat(structs::sAttackStat attackStat) { mItemAttackStat = attackStat; }
		structs::sAttackStat GetItemAttackStat() { return mItemAttackStat; }
		void SetItemScale(math::Vector3 scale) { mItemTransform->SetScale(scale); }
		math::Vector3 GetItemScale() { return mItemTransform->GetScale(); }
		void SetItemPosition(math::Vector3 position) { mItemTransform->SetPosition(position); }

	public:
		void SetItemUnitTypes(structs::sUnitTypes unitTypes) { mItemUnitTypes = unitTypes; }
		const structs::sUnitTypes GetItemUnitTypes() { return mItemUnitTypes; }
		void SetItemUnitInfo(const structs::sActionUnitInfo unitInfo) { mItemUnitInfo = unitInfo; }
		const structs::sActionUnitInfo GetItemUnitInfo() { return mItemUnitInfo; }		
		void SetItemUnitAnimation(structs::sAnimationInfo animationInfo) { mItemUnitAnimationInfo = animationInfo; }
		const structs::sAnimationInfo GetItemUnitAnimationInfo() { return mItemUnitAnimationInfo; }
		void SetItemUnitScale(math::Vector3 scale) { mItemUnitScale = scale; }
		math::Vector3 GetItemUnitScale() { return mItemUnitScale; }
		void SetItemUnitOffset(math::Vector3 offset) { mItemUnitOffset = offset; }
		math::Vector3 GetItemUnitOffset() { return mItemUnitOffset; }

	protected:
		Transform*					mItemTransform;
		MeshRenderer*				mItemRenderer;

	protected:
		std::wstring				mItemName;
		enums::eItemRenderType		mItemRenderType;
		structs::sAnimationInfo		mItemAnimationInfo;
		structs::sAttackStat		mItemAttackStat;

	protected:
		structs::sUnitTypes			mItemUnitTypes;
		structs::sActionUnitInfo	mItemUnitInfo;
		structs::sAnimationInfo		mItemUnitAnimationInfo;
		math::Vector3				mItemUnitScale;
		math::Vector3				mItemUnitOffset;

	};
}