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
		Grid,
		Life,
		Count,
		End,
	};

	enum class eSamplerType
	{
		Point,
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
		None,
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

	struct GPUBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
		D3D11_BUFFER_DESC desc;

		GPUBuffer()
			: Buffer(nullptr)
			, desc{}
		{}
		virtual ~GPUBuffer() = default;
	};
	struct DebugMesh
	{
		enums::eColliderType Type;
		math::Vector3 Position;
		math::Vector3 Rotation;
		math::Vector3 Scale;
		float Radius;
		float Duration;
		float Time;
	};
};