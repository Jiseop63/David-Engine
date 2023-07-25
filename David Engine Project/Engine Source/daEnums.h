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
		None, // 숭돌 없고 렌더 안하는 객체

		Background,	// 움직이지 않음
		Layer,		// 카메라에 영향을 받음

		Tile,
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
		None,
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

	enum class eColliderType
	{
		Rect,
		Circle,
		Sphere,
		Cube,
		End,
	};
	
	enum class eColliderColor
	{
		Magenta,
		Green,
		Red,
		
		End,
	};

#pragma region Contents
	enum class eWeaponType
	{
		None,
		LongSword,
	};
	enum class eShieldType
	{
		None,
		MagicShield,
	};
	enum class eAccessoryType
	{
		None,
		SilverRing,
	};
	enum class eItemType
	{
		None,
	};
#pragma endregion
}
