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

	enum class eLayerType
	{
		None, // ���� ���� ���� ���ϴ� ��ü
		Camera,
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

	enum class eComponentType
	{
		None,
		Transform,
		Rigidbody,
		TimeConstants,
		Mesh,
		Collider,
		Renderer,
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
	
}
