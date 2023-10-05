#include "daMath.h"
#include "daEnums.h"

namespace da::structs
{

	struct sAnimationInfo
	{
		std::wstring Name;
		bool		 Loop;
		sAnimationInfo() : Name(), Loop(true)
		{}
	};
	struct sUnitTypes
	{
		enums::eUnitActionType	ActionType;
		enums::eUnitUsageType	UsageType;
		enums::eUnitRenderType	RenderType;
		sUnitTypes() : ActionType(enums::eUnitActionType::None), UsageType(enums::eUnitUsageType::Default), RenderType(enums::eUnitRenderType::Stay)
		{}
	};

	struct sActionTimeValues
	{
		float Start;
		float End;
		sActionTimeValues() : Start(0.0f), End(1.0f)
		{}
		void Clear() { Start = 0.0f; }
		bool TimeOut() { return (End <= Start) ? true : false; }
	};
	struct sActionUnitInfo
	{
		float Speed;
		float Range;
		sActionTimeValues DurationTime;
		sActionUnitInfo() : Speed(0.0f), Range(0.0f), DurationTime{}
		{}
	};
	struct sAttackStat
	{
		float						AtaackDamage;
		sActionTimeValues			AttackDelayTime;

		sAttackStat()
			: AtaackDamage(0.0f)
			, AttackDelayTime{}
		{}
	};
	struct sItemInfo
	{
		enums::eItemRenderType	RenderType;
		sItemInfo() : RenderType(enums::eItemRenderType::Texture)
		{}
	};

	struct sCreatureStat
	{
		float MaxHP;
		float CurHP;
		float MoveSpeed;		
				
		sCreatureStat()
			: MaxHP(0.0f), CurHP(0.0f), MoveSpeed(0.0f)
		{}
	};

	struct sMonsterAttackStat
	{
		float AttackRange;
		float AttackDelayAccumulateTime;
		float AttackDelay;
		sMonsterAttackStat()
			: AttackRange(0.0f), AttackDelayAccumulateTime(0.0f), AttackDelay(0.0f)
		{}
	};
	struct sBossProjectileStat
	{
		float ProjectileDamage;
		float ProjectileRange;
		float ProjectileSpeed;
		float ProjectileAngle;
		bool ProjectileCollision;
		sBossProjectileStat()
			: ProjectileDamage(0.0f), ProjectileRange(0.0f), ProjectileSpeed(0.0f), ProjectileAngle(0.0f), ProjectileCollision(false)
		{}
	};
	struct sJumpData
	{
		float	JumpForce;
		float	JumpAccumulateTime;
		float	JumpLimitTime;
		float	JumpForceRatio;
		bool	BufferedJump;
		bool	ExtraJump;
		sJumpData()
			: JumpForce(0.0f), JumpAccumulateTime(0.0f), JumpLimitTime(0.0f), JumpForceRatio(0.0f)
			, BufferedJump(false), ExtraJump(true)
		{}
	};
	struct sDashData
	{
		int		MaxDashCount;
		int		CurDashCount;
		float	DashForce;
		float	DashAccumulateTime;
		float	DashRegenTime;
		sDashData()
			: MaxDashCount(0), CurDashCount(0), DashForce(0.0f), DashAccumulateTime(0.0f), DashRegenTime(0.0f)
		{}
	};

	struct sProjectileStat
	{
		std::wstring				EffectName;
		da::enums::eProjectileType	ProjectileType;
		float						ProjectileAngle;
		float						ProjectileCenterPadding;
		float						ProjectileValidTime;
		float						ProjectileValidAccumulateTime;
		float						ProjectileRange;
		float						ProjectileSpeed;
		bool						ProjectileActive;
		bool						ProjectileCollision;
		sProjectileStat()
			: EffectName()
			, ProjectileType(enums::eProjectileType::Melee)
			, ProjectileAngle(0.0f)
			, ProjectileCenterPadding(0.0f)
			, ProjectileValidTime(0.0f)
			, ProjectileValidAccumulateTime(0.0f)
			, ProjectileRange(0.0f)
			, ProjectileSpeed(0.0f)
			, ProjectileActive(false)
			, ProjectileCollision(false)
		{}
	};
	struct sWeaponInfo
	{
		da::enums::eWeaponName		WeaponName;
		std::wstring				AnimationName;
		bool						IsAnimationType;
		sAttackStat					AttackStat;
		sProjectileStat				ProjectileStat;
		
		sWeaponInfo()
			: WeaponName(da::enums::eWeaponName::Default)
			, IsAnimationType(false)
			, AttackStat{}
			, ProjectileStat{}
		{}
	};
	struct sShieldStat
	{
		da::enums::eShieldName	ShieldName;
		float					BlockPower;
		sShieldStat()
			: ShieldName(da::enums::eShieldName::Default), BlockPower(0.0f)
		{}
	};

	struct sArmour
	{
		sWeaponInfo Weapon;
		sShieldStat Shield;
		sArmour()
			: Weapon{}, Shield{}
		{}
	};
	struct sAccessorys
	{
		da::enums::eAccessoryType a;
		da::enums::eAccessoryType b;
		da::enums::eAccessoryType c;
		da::enums::eAccessoryType d;
		da::enums::eAccessoryType e;

		sAccessorys()
			: a(da::enums::eAccessoryType::Default), b(da::enums::eAccessoryType::Default)
			, c(da::enums::eAccessoryType::Default), d(da::enums::eAccessoryType::Default)
			, e(da::enums::eAccessoryType::Default)
		{}
	};
	struct sItems
	{
		da::enums::eItemType i00;
		da::enums::eItemType i01;
		da::enums::eItemType i02;
		da::enums::eItemType i03;
		da::enums::eItemType i04;

		da::enums::eItemType i10;
		da::enums::eItemType i11;
		da::enums::eItemType i12;
		da::enums::eItemType i13;
		da::enums::eItemType i14;

		da::enums::eItemType i20;
		da::enums::eItemType i21;
		da::enums::eItemType i22;
		da::enums::eItemType i23;
		da::enums::eItemType i24;

		sItems()
			: i00(da::enums::eItemType::Default), i01(da::enums::eItemType::Default)
			, i02(da::enums::eItemType::Default), i03(da::enums::eItemType::Default)
			, i04(da::enums::eItemType::Default)
			, i10(da::enums::eItemType::Default), i11(da::enums::eItemType::Default)
			, i12(da::enums::eItemType::Default), i13(da::enums::eItemType::Default)
			, i14(da::enums::eItemType::Default)
			, i20(da::enums::eItemType::Default), i21(da::enums::eItemType::Default)
			, i22(da::enums::eItemType::Default), i23(da::enums::eItemType::Default)
			, i24(da::enums::eItemType::Default)
		{}
	};
	struct sInventory
	{
		sArmour Armour1;
		sArmour Armour2;
		sAccessorys Accessorys;
		sItems Items;
		sInventory()
			: Armour1{}, Armour2{}, Accessorys{}, Items{}
		{}
	};
}