#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "daEnums.h"
#include "daMath.h"

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

#define CBSLOT_TRANSFORM		0;
#define CBSLOT_TIME				2;
#define CBSLOT_GRID				3;
#define CBSLOT_LIFE				4;
#define CBSLOT_COUNT			5;
#define CBSLOT_ANIMATION		6;
#define CBSLOT_REVERSE			7;
#define CBSLOT_COLLIDER			8;
#define CBSLOT_PARTICLE			9;

namespace da::graphics
{
	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		End,
	};

	enum class eCBType
	{
		Transform,
		Material,
		Time,
		Camera,
		Life,
		Count,
		Animation,
		Reverse,
		Collider,
		Particle,
		End,
	};

	enum class eSamplerType
	{
		Point,
		Linear,
		Anisotropic,
		End,
	};

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		Wireframe,
		End,
	};

	enum class eDSType
	{
		Less,
		Greater,
		NoWrite,
		Default,
		End,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		Oneone,
		End,
	};

	enum class eRenderingMode
	{
		Opaque,
		Cutout,
		Transparent,
		End,
	};

	enum class eViewType
	{
		None,
		SRV,
		UAV,
		End,
	};
	struct GPUBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
		D3D11_BUFFER_DESC Desc;

		GPUBuffer()
			: Buffer(nullptr)
			, Desc{}
		{}
		virtual ~GPUBuffer() = default;
	};

	struct DebugMesh
	{
		enums::eColliderShape Type;
		math::Vector3 Position;
		math::Vector3 Rotation;
		math::Vector3 Scale;
		math::Vector4 Color;

		float Radius;
		float Duration;
		float Time;
	};

	struct ParticleShared
	{
		UINT SharedActiveCount;
	};
#pragma region StructuredBuffer

	struct LightAttribute
	{
		math::Vector4		Color;
		math::Vector4		Position;
		math::Vector4		Direction;

		enums::eLightType   LightType;
		float				LightRadius;
		float				LightAngle;
		int					Padding;
	};

	struct Particle
	{
		math::Vector4 Position;
		math::Vector4 Direction;

		float	EndTime;
		float	Time;
		float	Speed;
		UINT	Active;
	};
#pragma endregion

};