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
		End,
	};

	enum class eLayerType
	{
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