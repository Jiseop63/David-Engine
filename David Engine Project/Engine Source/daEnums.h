#pragma once

namespace da::enums
{
	enum class eResourceType
	{
		Mesh,
		Texture,
		Shader,
		Material,
		End,
	};

	enum class eComponentType
	{
		None,
		Transform,
		Rigidbody,
		TimeConstants,
		Mesh,
		Renderer,
		Particle,
		Script,
		Camera,
		End,
	};

	enum class eLayerType
	{
		None, // 숭돌 없고 렌더 안하는 객체
		BackGround,	// 움직이지 않음
		Layer,		// 카메라에 영향을 받음
		Tile,
		ENV,
		Playable,
		Creature,
		Effect,
		UI,
		Button,
		End,
	};

	enum class eSceneType
	{
		Title,
		Select,
		Play,
		End,
	};
}