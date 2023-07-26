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
	struct sDashCount
	{
		int MaxCount;
		int CurCount;
		sDashCount()
			: MaxCount(0), CurCount(0)
		{}
	};

	struct sArmour
	{
		da::enums::eWeaponType Weapon;
		da::enums::eShieldType Shield;
		sArmour()
			: Weapon(da::enums::eWeaponType::Default), Shield(da::enums::eShieldType::Default)
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