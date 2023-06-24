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
		None, // ¼þµ¹ ¾ø°í ·»´õ ¾ÈÇÏ´Â °´Ã¼
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