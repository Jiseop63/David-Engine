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
		Mesh,
		Renderer,
		Particle,
		Script,
		Camera,
		End,
	};

	enum class eLayerType
	{
		None, // ¼þµ¹ ¾ø°í ·»´õ ¾ÈÇÏ´Â °´Ã¼
		BackGround,
		a,
		b,
		c,
		d,
		e,
		Tile,
		Playable,
		Enemy,
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