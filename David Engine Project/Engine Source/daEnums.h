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
		None, // ���� ���� ���� ���ϴ� ��ü
		BackGround,	// �������� ����
		Layer,		// ī�޶� ������ ����
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