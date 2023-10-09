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
		ComputeShader,
		AudioClip,
		End,
	};

	enum class eLayerType
	{
		Default, // ���� ���� ���� ���ϴ� ��ü

		// BG
		Background,		// �������� ����
		Layer,			// ī�޶� ������ ����
		FrontBackGround,

		// Env Collision
		Land,
		Platform,

		// Object Collision
		ENV,
		Portal,

		Monster,
		MonsterProjectile,
		Boss,
		BossProjectile,
		
		Playable,
		PlayableProjectile,

		// Graphic
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
		AudioListener,
		AudioSource,
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
	enum class eItemSlot
	{
		Slot00, Slot01, Slot02, Slot03,			// weapon & Shiled
		Slot10, Slot11, Slot12, Slot13,			// accessory
		Slot20, Slot21, Slot22, Slot23, Slot24,	// etc
		Slot30, Slot31, Slot32, Slot33, Slot34,	// etc
		Slot40, Slot41, Slot42, Slot43, Slot44,	// etc
		End,
	};

	// ����ü ����
	enum class eUnitUsageType
	{
		JustAnimation		,
		JustTexture			,
		
		TextureProjectile	,
		AnimationProjectile	,
		InvisibleProjectile	,
	};
	enum class eUnitLifeType
	{
		Duration,
		Range,
		AnimationEnd,
	};
	enum class eUnitActionType
	{
		Stay,				// dont move
		UsingDirection,		// move to dir
		UsingRotation,		// move to rotate
	};


	enum class eItemRenderType
	{
		Texture,
		Animation,
	};

	enum class eItemAttackType
	{
		Swing,		// �� ���ݽ� ��->�Ʒ�->�� �ֵθ��� ����
		Shoot,		// ���ݽ� �ݵ��� �޾� ���� ��鸮�� ����
		Spin,		// �������̸� ���� ������ ����
		None,
	};


	// �Ⱦ� ����
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

	// �Ⱦ�����
	enum class eProjectileType
	{
		Melee,
		Range,
		Body,
	};
#pragma endregion
}
