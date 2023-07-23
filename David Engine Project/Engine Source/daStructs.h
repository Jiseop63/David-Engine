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
		float MaxCount;
		float CurCount;
		sDashCount()
			: MaxCount(0), CurCount(0)
		{}
	};

	struct sArmour
	{
		da::enums::eWeaponType Weapon;
		da::enums::eShieldType Shield;
		sArmour()
			: Weapon(da::enums::eWeaponType::None), Shield(da::enums::eShieldType::None)
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
			: a(da::enums::eAccessoryType::None), b(da::enums::eAccessoryType::None)
			, c(da::enums::eAccessoryType::None), d(da::enums::eAccessoryType::None)
			, e(da::enums::eAccessoryType::None)
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
			: i00(da::enums::eItemType::None), i01(da::enums::eItemType::None)
			, i02(da::enums::eItemType::None), i03(da::enums::eItemType::None)
			, i04(da::enums::eItemType::None)
			, i10(da::enums::eItemType::None), i11(da::enums::eItemType::None)
			, i12(da::enums::eItemType::None), i13(da::enums::eItemType::None)
			, i14(da::enums::eItemType::None)
			, i20(da::enums::eItemType::None), i21(da::enums::eItemType::None)
			, i22(da::enums::eItemType::None), i23(da::enums::eItemType::None)
			, i24(da::enums::eItemType::None)
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