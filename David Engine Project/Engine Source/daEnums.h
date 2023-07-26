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

		Playable,
		PlayableAttackCollider,
		Creature,
		CreatureAttackCollider,

		Effect,
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
	

#pragma region Contents
	enum class eWeaponType
	{
		Default,
		LongSword,
	};
	enum class eShieldType
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
#pragma endregion
}
