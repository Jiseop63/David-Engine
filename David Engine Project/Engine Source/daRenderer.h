#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"
#include "daMesh.h"
#include "daShader.h"
#include "daConstantBuffer.h"

namespace renderer
{
	struct Vertex
	{
		da::math::Vector3 pos;
		da::math::Vector4 color;
		da::math::Vector2 uv;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		da::math::Matrix mWorld;
		da::math::Matrix mView;
		da::math::Matrix mProjection;
	};

	extern Vertex vertexes[];
	extern da::graphics::ConstantBuffer* constantBuffer[(UINT)da::graphics::eCBType::End];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)da::graphics::eSamplerType::End];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates[(UINT)da::graphics::eRSType::End];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStates[(UINT)da::graphics::eDSType::End];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates[(UINT)da::graphics::eBSType::End];

	void Initialize();
	void Release();
}
