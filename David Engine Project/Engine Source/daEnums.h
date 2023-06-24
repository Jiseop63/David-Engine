#pragma once

namespace da::enums
{
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
		Playable,
		Enemy,
		UI,
		End,
	};

	enum class eResourceType
	{
		Mesh,
		Texture,
		Shader,
		Material,
		End,
	};
}