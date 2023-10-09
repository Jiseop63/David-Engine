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
		Default, // 숭돌 없고 렌더 안하는 객체

		// BG
		Background,		// 움직이지 않음
		Layer,			// 카메라에 영향을 받음
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

	// 투사체 정보
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
		Swing,		// 매 공격시 위->아래->위 휘두르는 패턴
		Shoot,		// 공격시 반동을 받아 살작 흔들리는 패턴
		Spin,		// 공격중이면 빙빙 돌리는 패턴
		None,
	};


	// 안쓸 예정
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

	// 안쓸예정
	enum class eProjectileType
	{
		Melee,
		Range,
		Body,
	};
#pragma endregion
}
