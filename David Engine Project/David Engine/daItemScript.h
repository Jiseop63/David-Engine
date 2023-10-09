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

		// visual
	public:
		void SetItemTexture(std::shared_ptr<graphics::Texture> texture) 
		{ 
			mItemTexture = texture;
			mItemRenderer->GetMaterial()->SetTexture(mItemTexture);
		}
		std::shared_ptr<graphics::Texture> GetItemTexture() { return mItemTexture; }
		void SetItemScale(math::Vector3 itemScale)
		{
			mItemScale = itemScale;
			mItemTransform->SetScale(mItemScale);
		}
		math::Vector3 GetItemScale() { return mItemScale; }
		void SetItemOffset(math::Vector3 itemOffset) { mItemOffset = itemOffset; }
		math::Vector3 GetItemOffset() { return mItemOffset; }
		// item function
	public:
		void SetItemInfo(structs::sItemInfo itemInfo) { mItemInfo = itemInfo; }
		structs::sItemInfo GetItemInfo() { return mItemInfo; }

		void SetItemPosition(math::Vector3 position) { mItemTransform->SetPosition(position); }

		// projectile
	public:
		void SetProjectileTypes(structs::sActionUnitTypes projectileTypes) { mProjectileTypes = projectileTypes; }
		structs::sActionUnitTypes GetProjectileTypes() { return mProjectileTypes; }

		void SetProjectileStat(structs::sActionUnitStat projectileStat) { mProjectileStat = projectileStat; }
		structs::sActionUnitStat GetProjectileStat() { return mProjectileStat; }

		void SetProjectileScale(math::Vector3 scale) { mProjectileScale = scale; }
		math::Vector3 GetProjectileScale() { return mProjectileScale; }
		void SetProjectileOffset(math::Vector3 offset) { mProjectileOffset = offset; }
		math::Vector3 GetProjectileOffset() { return mProjectileOffset; }
		void SetProjectileSize(math::Vector2 size) { mProjectileSize = size; }
		math::Vector2 GetProjectileSize() { return mProjectileSize; }


	protected:
		Transform*					mItemTransform;
		MeshRenderer*				mItemRenderer;

		// 이거 필요없는거같음
		std::wstring				mItemName;

		// visual
	protected:
		std::shared_ptr<Texture>	mItemTexture;
		math::Vector3				mItemScale;
		math::Vector3				mItemOffset;

		// function
	protected:
		structs::sItemInfo			mItemInfo;

		// projectile
	protected:
		structs::sActionUnitTypes	mProjectileTypes;
		structs::sActionUnitStat	mProjectileStat;

		math::Vector3				mProjectileScale;	// Transform
		math::Vector3				mProjectileOffset;	// Transform
		math::Vector2				mProjectileSize;	// collider
	};
}