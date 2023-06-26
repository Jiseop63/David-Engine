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
		None, // ���� ���� ���� ���ϴ� ��ü
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