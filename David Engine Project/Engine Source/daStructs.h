#include "daMath.h"
namespace da::structs
{
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
		float DetectRange;
		float AttackRange;
		float AttackDelayAccumulateTime;
		float AttackDelay;
		sMonsterAttackStat()
			: DetectRange(0.0f), AttackRange(0.0f), AttackDelayAccumulateTime(0.0f), AttackDelay(0.0f)
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
	struct sAttackStat
	{
		bool						AttackReady;
		float						AtaackDamage;
		float						AttackDelayAccumulateTime;
		float						AttackDelayTime;
		sAttackStat()
			: AttackReady(true)
			, AtaackDamage(0.0f)
			, AttackDelayAccumulateTime(0.0f)
			, AttackDelayTime(0.0f)
		{}
	};
	struct sProjectileStat
	{
		bool						ProjectileActive;
		bool						ProjectileCollision;
		float						ProjectileRange;
		float						ProjectileValidTime;
		float						ProjectileAccumulate;
		float						ProjectileCenterPadding;
		da::enums::eProjectileType	ProjectileType;
		sProjectileStat()
			: ProjectileActive(false)
			, ProjectileCollision(false)
			, ProjectileRange(0.0f)
			, ProjectileValidTime(0.0f)
			, ProjectileAccumulate(0.0f)
			, ProjectileCenterPadding(0.0f)
			, ProjectileType(enums::eProjectileType::Melee)
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