#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"
#include "daConstantBuffer.h"
#include "daCamera.h"

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
		da::math::Matrix World;
		da::math::Matrix View;
		da::math::Matrix Projection;
	};
	CBUFFER(TimeCB, CBSLOT_TIME)
	{
		float ElapsedTime;
		float Value;
	};

	extern Vertex vertexes[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)da::graphics::eSamplerType::End];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates[(UINT)da::graphics::eRSType::End];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStates[(UINT)da::graphics::eDSType::End];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates[(UINT)da::graphics::eBSType::End];
	extern da::graphics::ConstantBuffer* constantBuffer[(UINT)da::graphics::eCBType::End];
	extern std::vector<da::Camera*> cameras;
	
	extern da::Camera* mainCamera;
	extern da::Camera* uiCamera;

	void Initialize();
	void Render();
	void Release();
}
