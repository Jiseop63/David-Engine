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
	enum class eUnitActionType
	{
		Duration,
		Range,
		None,
	};
	enum class eUnitRenderType
	{
		Stay,				// dont move
		JustRotate,			// dont move & rotate
		UsingDirection,		// move to dir
		UsingRotation,		// move to rotate
	};
	enum class eUnitUsageType
	{
		Default,			// Animation & Collider
		OnlyTexture,		// Texture 
		OnlyAnimation,		// Animation
		OnlyCollider,		// Collider
	};

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
