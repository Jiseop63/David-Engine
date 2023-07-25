#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"
#include "daConstantBuffer.h"
#include "daCamera.h"
#include "..\\David Engine\daGridScript.h"

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

	CBUFFER(CameraCB, CBSLOT_GRID)
	{
		da::math::Vector4 CameraPosition;
		da::math::Vector2 CameraScale;
		da::math::Vector2 Resolution;
	};
	CBUFFER(LifeCB, CBSLOT_LIFE)
	{
		float MaxHP;
		float CurHP;
	};
	CBUFFER(CountCB, CBSLOT_COUNT)
	{
		float MaxCount;
		float CurCount;
	};
	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		da::math::Vector2 SpriteLeftTop;
		da::math::Vector2 SpriteSize;
		da::math::Vector2 SpriteOffset;
		da::math::Vector2 AtlasSize;
	};	
	CBUFFER(ReverseCB, CBSLOT_REVERSE)
	{
		bool Reverse;
	};

	CBUFFER(ColliderCB, CBSLOT_COLLIDER)
	{
		UINT ColliderColorType;
	};


	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)da::graphics::eSamplerType::End];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates[(UINT)da::graphics::eRSType::End];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStates[(UINT)da::graphics::eDSType::End];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates[(UINT)da::graphics::eBSType::End];

	extern da::graphics::ConstantBuffer* constantBuffer[(UINT)da::graphics::eCBType::End];
	
	extern std::vector<da::Camera*> cameras;
	extern std::vector<da::graphics::DebugMesh> debugMeshs;
	
	extern da::Camera* mainCamera;
	extern da::Camera* uiCamera;
	extern da::GridScript* gridScript;

	void Initialize();
	void Render();
	void Release();
	void PushDebugMeshAttribute(da::graphics::DebugMesh mesh);
}
