#pragma once

namespace da::enums
{
	enum class eResourceType
	{
		Mesh,
		Texture,
		Shader,
		Material,
		Animation,
		End,
	};

	enum class eLayerType
	{
		Default, // ���� ���� ���� ���ϴ� ��ü

		Background,	// �������� ����
		Layer,		// ī�޶� ������ ����
		FrontBackGround,
		Land,
		ENV,

		Creature,
		CreatureAttackCollider,
		Effect,
		Playable,
		PlayableAttackCollider,

		Light,
		UI,
		Button,
		End,
	};

	enum class eComponentType
	{
		Default,
		Transform,
		Rigidbody,
		TimeConstants,
		Mesh,
		Collider,
		Renderer,
		Animator,
		Particle,
		Script,
		Light,
		Camera,
		End,
	};

	enum class eColliderShape
	{
		Rect,
		Circle,
		Sphere,
		Cube,
		End,
	};
	
	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		End,
	};
#pragma region Contents
	enum class eWeaponName
	{
		Default,
		LongSword,
	};
	enum class eShieldName
	{
		Default,
		MagicShield,
	};
	enum class eAccessoryType
	{
		Default,
		SilverRing,
	};
	enum class eItemType
	{
		Default,
	};

	enum class eProjectileType
	{
		Melee,
		Range,
		Body,
	};
#pragma endregion
}
