#include "daRenderer.h"
#include "daResources.h"
#include "daMesh.h"
#include "daTexture.h"
#include "daShader.h"
#include "daMaterial.h"
#include "daStructuredBuffer.h"
#include "daPaintShader.h"
#include "daParticleShader.h"
#include "daAudioClip.h"
using namespace da;
using namespace da::graphics;
using namespace da::math;

namespace renderer
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates[(UINT)eBSType::End] = {};

	ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
	StructuredBuffer* lightsBuffer = nullptr;
	std::vector<Camera*> cameras = {};
	std::vector<DebugMesh> debugMeshs = {};
	std::vector<Light*> lights = {};

	Camera* mainCamera = nullptr;
	Camera* uiCamera = nullptr;
	GridScript* gridScript = nullptr;

	void LoadMesh()
	{
		std::vector<Vertex> vertexes = {};
		std::vector<UINT> indexes = {};
#pragma region Point Mesh
		Vertex point = {};
		point.pos = Vector3(0.0f, 0.0f, 0.0f);
		vertexes.push_back(point);
		indexes.push_back(0);
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
		Resources::Insert(L"PointMesh", mesh);


		vertexes.clear();
		indexes.clear();
#pragma endregion

#pragma region Rect Mesh
		vertexes.resize(4);
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"RectMesh", mesh);
		mesh->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());

		indexes.push_back(0); indexes.push_back(1); indexes.push_back(2);
		indexes.push_back(0); indexes.push_back(2); indexes.push_back(3);

		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
#pragma endregion
#pragma region Debug Mesh (rect, circle)
		// Rect Debug Mesh
		std::shared_ptr<Mesh> rectDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugRect", rectDebug);
		rectDebug->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		rectDebug->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

		// Circle Debug Mesh
		vertexes.clear();
		indexes.clear();

		Vertex center = {};
		center.pos = Vector3(0.0f, 0.0f, 0.0f);
		center.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes.push_back(center);

		int iSlice = 40;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / (float)iSlice;

		for (int i = 0; i < iSlice; ++i)
		{
			center.pos = Vector3(fRadius * cosf(fTheta * (float)i)
				, fRadius * sinf(fTheta * (float)i)
				, 0.0f);
			center.color = Vector4(0.0f, 1.0f, 0.0f, 1.f);
			vertexes.push_back(center);
		}

		for (int i = 0; i < vertexes.size() - 2; ++i)
		{
			indexes.push_back(i + 1);
		}
		indexes.push_back(1);

		std::shared_ptr<Mesh> circleDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugCircle", circleDebug);
		circleDebug->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		circleDebug->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
#pragma endregion
	}
	void LoadBuffer()
	{
#pragma region Create constant buffer 

		// 추가할 cbuffer 목록
		constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffer[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		// background
		constantBuffer[(UINT)eCBType::Time] = new ConstantBuffer(eCBType::Time);
		constantBuffer[(UINT)eCBType::Time]->Create(sizeof(TimeCB));

		// Grid
		constantBuffer[(UINT)eCBType::Camera] = new ConstantBuffer(eCBType::Camera);
		constantBuffer[(UINT)eCBType::Camera]->Create(sizeof(CameraCB));

		// BarValue
		constantBuffer[(UINT)eCBType::Life] = new ConstantBuffer(eCBType::Life);
		constantBuffer[(UINT)eCBType::Life]->Create(sizeof(LifeCB));
		constantBuffer[(UINT)eCBType::Count] = new ConstantBuffer(eCBType::Count);
		constantBuffer[(UINT)eCBType::Count]->Create(sizeof(CountCB));

		// Animator
		constantBuffer[(UINT)eCBType::Animation] = new ConstantBuffer(eCBType::Animation);
		constantBuffer[(UINT)eCBType::Animation]->Create(sizeof(AnimationCB));

		// Reverse
		constantBuffer[(UINT)eCBType::Reverse] = new ConstantBuffer(eCBType::Reverse);
		constantBuffer[(UINT)eCBType::Reverse]->Create(sizeof(ReverseCB));

		// ColliderColor
		constantBuffer[(UINT)eCBType::Collider] = new ConstantBuffer(eCBType::Collider);
		constantBuffer[(UINT)eCBType::Collider]->Create(sizeof(ColliderCB));
		
		// light structed buffer
		lightsBuffer = new StructuredBuffer();
		lightsBuffer->Create(sizeof(LightAttribute), 2, eViewType::SRV, nullptr, true);

		// Particle
		constantBuffer[(UINT)eCBType::Particle] = new ConstantBuffer(eCBType::Particle);
		constantBuffer[(UINT)eCBType::Particle]->Create(sizeof(ParticleCB));

		// Noise
		constantBuffer[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
		constantBuffer[(UINT)eCBType::Noise]->Create(sizeof(NoiseCB));

		// fade

		// afterimage

		// blink

#pragma endregion
	}
	void LoadShader()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		// Default Shaders
		{
			shader->Create(eShaderStage::VS, L"TriangleShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"TriangleShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"TriangleShader", shader);
		}
		// sprite
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"SpriteShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"SpriteShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"SpriteShader", shader);
		}
		// anim
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"AnimationShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"AnimationShader.hlsl", "mainPS");
			Resources::Insert(L"AnimationShader", shader);
		}
		// Rotate anim
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"RotateAnimationShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"RotateAnimationShader.hlsl", "mainPS");
			Resources::Insert(L"RotateAnimationShader", shader);
		}
		// Invisible
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"InvisibleShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"InvisibleShader.hlsl", "mainPS");
			Resources::Insert(L"InvisibleShader", shader);
		}
		// Background Shaders
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"MovingBGShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"MovingBGShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"MovingBGShader", shader);
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"TilingLayerShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"TilingLayerShader.hlsl", "mainPS");
			Resources::Insert(L"TilingLayerShader", shader);
		}
		// Grid Shader
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"GridShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"GridShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"GridShader", shader);
		}
		// slide bar
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"LifeBarShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"LifeBarShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"LifeBarShader", shader);
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"DashCountShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"DashCountShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"DashCountShader", shader);
		}
		// debug
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"TriangleShader.hlsl", "mainVS");
			shader->Create(eShaderStage::PS, L"DebugShader.hlsl", "mainPS");
			shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			shader->SetRatserizerState(eRSType::Wireframe);
			Resources::Insert(L"DebugShader", shader);
		}
		// paritlce
		{
			shader = std::make_shared<Shader>();
			shader->Create(eShaderStage::VS, L"ParticleShader.hlsl", "mainVS");
			shader->Create(eShaderStage::GS, L"ParticleShader.hlsl", "mainGS");
			shader->Create(eShaderStage::PS, L"ParticleShader.hlsl", "mainPS");
			shader->SetRatserizerState(eRSType::SolidNone);
			shader->SetDepthStencilState(eDSType::NoWrite);
			shader->SetBlendState(eBSType::AlphaBlend);
			shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			Resources::Insert(L"ParticleShader", shader);
		}

		// paint
		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"Paint.hlsl", "mainCS");
		da::Resources::Insert(L"PaintShader", paintShader);

		std::shared_ptr<ParticleShader> particleShader = std::make_shared<ParticleShader>();
		particleShader->Create(L"ParticleShader.hlsl", "mainCS");
		da::Resources::Insert(L"ParticleComputeShader", particleShader);
	}
	void SetupState()
	{
#pragma region InputLayout
		const int numElement = 3;
		D3D11_INPUT_ELEMENT_DESC arrLayout[numElement] = {};

		arrLayout[0].AlignedByteOffset = 0;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 12;
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		arrLayout[2].AlignedByteOffset = 28;
		arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TEXCOORD";
		arrLayout[2].SemanticIndex = 0;

		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"TriangleShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
		
		shader = Resources::Find<Shader>(L"SpriteShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"MovingBGShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"GridShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"LifeBarShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
		shader = Resources::Find<Shader>(L"DashCountShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"DebugShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"TilingLayerShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"AnimationShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"ParticleShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"InvisibleShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"RotateAnimationShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
		

#pragma endregion
#pragma region Sampler State

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSamplers(eShaderStage::PS, 0, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf()); 
		GetDevice()->BindSamplers(eShaderStage::PS, 1, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSamplers(eShaderStage::PS, 2, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		// Soild Back	: 뒷면을 렌더링하지 않음
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());
		// Soild Front	: 앞면을 렌더링하지 않음
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());
		// Solid None	: 모든 면을 렌더링함
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());
		// Wireframe	: 뼈대만 렌더링함
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::Wireframe].GetAddressOf());
#pragma endregion
#pragma region DepthStencil State
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		// Less <- Z 값이 나보다 작으면 안그림
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::Less].GetAddressOf());
		// Greater <- Z 값이 나보다 크면 안그림
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::Greater].GetAddressOf());
		// No Write <- 덮어쓰기 안함
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());
		// None
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
#pragma region Blend State
		D3D11_BLEND_DESC blendDesc = {};
		// Default
		BlendStates[(UINT)eBSType::Default] = nullptr;
		// Alpha Blend
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(
			&blendDesc, BlendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());
		// Oneone
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(
			&blendDesc, BlendStates[(UINT)eBSType::Oneone].GetAddressOf());
#pragma endregion
	}
	void LoadTexture()
	{
		Resources::Load<Texture>(L"SampleTexture", L"..\\Resources\\Texture\\Sample\\smileTexture.png");
		Resources::Load<Texture>(L"Link", L"..\\Resources\\Texture\\Link.png");
		Resources::Load<Texture>(L"LinkSprite", L"..\\Resources\\Texture\\linkSprites.png");
		Resources::Load<Texture>(L"circleParticleTexture", L"..\\Resources\\Texture\\particle\\AlphaCircle.png");
		Resources::Load<Texture>(L"Noise01", L"..\\Resources\\Texture\\noise\\noise_01.png");
		Resources::Load<Texture>(L"Noise02", L"..\\Resources\\Texture\\noise\\noise_02.png");
		Resources::Load<Texture>(L"Noise03", L"..\\Resources\\Texture\\noise\\noise_03.png");

		std::shared_ptr<Texture> uavTexture = std::make_shared<Texture>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
		Resources::Insert(L"PaintTexture", uavTexture);

		// ui
		Resources::Load<Texture>(L"BasicCursorTexture", L"..\\Resources\\Texture\\Hud\\Mouse\\BasicCursor.png");
		Resources::Load<Texture>(L"ShootingCursorTexture", L"..\\Resources\\Texture\\Hud\\Mouse\\ShootingCursor2.png");
		Resources::Load<Texture>(L"PlayerLifePanelTexture", L"..\\Resources\\Texture\\Hud\\Life\\PlayerLifePanel.png");
		Resources::Load<Texture>(L"PlayerLifeBarTexture", L"..\\Resources\\Texture\\Hud\\Life\\LifeBar.png");
		Resources::Load<Texture>(L"DashPanelTexture", L"..\\Resources\\Texture\\Hud\\DashCount\\DashPanel.png");
		Resources::Load<Texture>(L"DashActivateTexture", L"..\\Resources\\Texture\\Hud\\DashCount\\DashCountsActivate.png");
		Resources::Load<Texture>(L"Armour1Texture", L"..\\Resources\\Texture\\Hud\\Armour\\Armour1.png");
		Resources::Load<Texture>(L"Armour2Texture", L"..\\Resources\\Texture\\Hud\\Armour\\Armour2.png");
		Resources::Load<Texture>(L"GreatSwordIconTexture", L"..\\Resources\\Texture\\Items\\GreatSword.png");

		Resources::Load<Texture>(L"KeyFTexture", L"..\\Resources\\Texture\\InputIcon\\Keyboard_F.png");


		// inventory
		Resources::Load<Texture>(L"InventoryPanelATexture", L"..\\Resources\\Texture\\Inventory\\InventoryPanelA.png");
		Resources::Load<Texture>(L"InventoryPanelBTexture", L"..\\Resources\\Texture\\Inventory\\InventoryPanelB.png");
		Resources::Load<Texture>(L"WeaponSlotTexture", L"..\\Resources\\Texture\\Inventory\\weapon.png");
		Resources::Load<Texture>(L"WeaponSlotSelectTexture", L"..\\Resources\\Texture\\Inventory\\weaponSelect.png");
		Resources::Load<Texture>(L"ShiledSlotTexture", L"..\\Resources\\Texture\\Inventory\\shiled.png");
		Resources::Load<Texture>(L"ShiledSlotSelectTexture", L"..\\Resources\\Texture\\Inventory\\shiledSelect.png");
		Resources::Load<Texture>(L"AccessorySlotTexture", L"..\\Resources\\Texture\\Inventory\\accessory.png");
		Resources::Load<Texture>(L"AccessorySlotSelectTexture", L"..\\Resources\\Texture\\Inventory\\accessorySelect.png");
		Resources::Load<Texture>(L"ItemSlotTexture", L"..\\Resources\\Texture\\Inventory\\ItemSlot.png");
		Resources::Load<Texture>(L"ItemSlotSelectTexture", L"..\\Resources\\Texture\\Inventory\\ItemSlotSelect.png");
		
		// item (안씀)
		Resources::Load<Texture>(L"LongSwordTexture", L"..\\Resources\\Texture\\Items\\LongSword.png");
		Resources::Load<Texture>(L"LongSwordTestTexture", L"..\\Resources\\Texture\\Items\\LongSwordTest.png");
		Resources::Load<Texture>(L"GreatSwordTexture", L"..\\Resources\\Texture\\Items\\GreatSword.png");
		Resources::Load<Texture>(L"GreatSword0Texture", L"..\\Resources\\Texture\\Items\\GreatSword0.png");
		Resources::Load<Texture>(L"GreatSword1Texture", L"..\\Resources\\Texture\\Items\\GreatSword1.png");

		// items
		Resources::Load<Texture>(L"LongSword", L"..\\Resources\\Texture\\Items\\LongSword.png");
		Resources::Load<Texture>(L"Crossbow", L"..\\Resources\\Texture\\Items\\CrossBow\\CrossbowIdle.png");
		Resources::Load<Texture>(L"CrossbowIdle", L"..\\Resources\\Texture\\Items\\CrossBow\\CrossbowIdle.png");
		Resources::Load<Texture>(L"CrossbowShot", L"..\\Resources\\Texture\\Items\\CrossBow\\CrossbowShot.png");
		Resources::Load<Texture>(L"CrossbowArrow", L"..\\Resources\\Texture\\Items\\CrossBow\\CrossbowArrow.png");
		Resources::Load<Texture>(L"QuarterStaff", L"..\\Resources\\Texture\\Items\\QuarterStaff\\QuarterStaff_Big.png");
		

		// effect
		Resources::Load<Texture>(L"GreatSwingFX", L"..\\Resources\\Texture\\Items\\GreatSword\\GreatSwingFX.png");
		Resources::Load<Texture>(L"SwingFX", L"..\\Resources\\Texture\\Adventurer\\FX\\SwingFX.png");
		Resources::Load<Texture>(L"JumpFX", L"..\\Resources\\Texture\\Adventurer\\FX\\JumpFX.png");
		Resources::Load<Texture>(L"DustFX", L"..\\Resources\\Texture\\Adventurer\\FX\\DustFX.png");
		Resources::Load<Texture>(L"SlashFX", L"..\\Resources\\Texture\\Adventurer\\FX\\SlashFX.png");
		Resources::Load<Texture>(L"DyingFX", L"..\\Resources\\Texture\\Adventurer\\FX\\DyingFX.png");
		Resources::Load<Texture>(L"DashEffect", L"..\\Resources\\Texture\\Adventurer\\DashEffect.png");
		Resources::Load<Texture>(L"HitEffect", L"..\\Resources\\Texture\\FX\\Arrow\\ArrowFX.png");



		// title
		Resources::Load<Texture>(L"TitleBackgroundTexture", L"..\\Resources\\Texture\\Scene_Title\\TitleBackground.png");
		Resources::Load<Texture>(L"BackCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\BackCloud.png");
		Resources::Load<Texture>(L"FrontCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\FrontCloud.png");
		Resources::Load<Texture>(L"SmallCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\smallCloud.png");
		Resources::Load<Texture>(L"MainLogoTexture", L"..\\Resources\\Texture\\Scene_Title\\MainLogo.png");
		Resources::Load<Texture>(L"CopyrightTexture", L"..\\Resources\\Texture\\Scene_Title\\Copyright.png");
		// title button
		Resources::Load<Texture>(L"PlayOffTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\PlayOff_Kor.png");
		Resources::Load<Texture>(L"PlayOnTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\PlayOn_Kor.png");
		Resources::Load<Texture>(L"ExitOffTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\ExitOff_Kor.png");
		Resources::Load<Texture>(L"ExitOnTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\ExitOn_Kor.png");
		Resources::Load<Texture>(L"SkyTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\Sky_Day.png");
		// town
		Resources::Load<Texture>(L"TownBGTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\TownBG_Day.png");
		Resources::Load<Texture>(L"TownLayerDayTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\TownLayer_Day.png");
		Resources::Load<Texture>(L"TownMapTileTexture", L"..\\Resources\\Texture\\Scene_Town\\TownMapTile.png");
		Resources::Load<Texture>(L"SecondFloor_OneWayTexture", L"..\\Resources\\Texture\\Scene_Town\\SecondFloor_OneWay.png");
		// 1F
		Resources::Load<Texture>(L"Platform1FTexture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\Platform1F.png");
		Resources::Load<Texture>(L"Platform1Fx4Texture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\Platform1Fx4.png");
		Resources::Load<Texture>(L"DungeonSubBGTexture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\SubBG.png");
		Resources::Load<Texture>(L"Close1FTexture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\close1F.png");
		Resources::Load<Texture>(L"Open1FTexture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\open1F.png");

		// dungeon etc
		Resources::Load<Texture>(L"FairyTexture", L"..\\Resources\\Texture\\Object\\FairyL.png");
		// light, box 추가하면 좋을거같은데


		// monster
		Resources::Load<Texture>(L"SkelSprite", L"..\\Resources\\Texture\\Monster\\Skel\\SpriteSheet.png");
		Resources::Load<Texture>(L"SkelHead", L"..\\Resources\\Texture\\Monster\\Skel\\SkelHead.png");
		
		Resources::Load<Texture>(L"EnemyGreatSword", L"..\\Resources\\Texture\\Monster\\Skel\\GreatSwordSpriteSheet.png");

		Resources::Load<Texture>(L"BansheeSpriteSheet", L"..\\Resources\\Texture\\Monster\\Banshee\\BansheeSpriteSheet.png");
		Resources::Load<Texture>(L"BansheeDead", L"..\\Resources\\Texture\\Monster\\Banshee\\BansheeDead.png");
		Resources::Load<Texture>(L"BansheeBulletIdle", L"..\\Resources\\Texture\\Monster\\Banshee\\BansheeBulletIdle.png");
		Resources::Load<Texture>(L"BansheeBulletHit", L"..\\Resources\\Texture\\Monster\\Banshee\\BansheeBulletHit.png");
		
		Resources::Load<Texture>(L"BossSpriteSheet", L"..\\Resources\\Texture\\SkellBoss\\SkellBossSpriteSheet.png");
		Resources::Load<Texture>(L"SkellBossAttacking", L"..\\Resources\\Texture\\SkellBoss\\SkellBossAttacking.png");
		
		Resources::Load<Texture>(L"SkellBossHandIdle", L"..\\Resources\\Texture\\SkellBoss\\SkellBossHandIdle.png");
		Resources::Load<Texture>(L"SkellBossHandAttack", L"..\\Resources\\Texture\\SkellBoss\\SkellBossHandAttack.png");
		Resources::Load<Texture>(L"SkellBossProjectile", L"..\\Resources\\Texture\\SkellBoss\\SkellBossProjectile.png");
		Resources::Load<Texture>(L"SkellBossLaser", L"..\\Resources\\Texture\\SkellBoss\\SkellBossLaser.png");
		//Resources::Load<Texture>(L"BossSpriteSheet", L"..\\Resources\\Texture\\Scene_Dungeon2F\\SkellBossSpriteSheet.png"); 칼
		
		// portal
		Resources::Load<Texture>(L"DungeonEat", L"..\\Resources\\Texture\\Scene_Town\\DungeonEat.png");
		
		Resources::Load<Texture>(L"SteleClose", L"..\\Resources\\Texture\\Dungeon\\SteleClose.png");
		Resources::Load<Texture>(L"SteleIdle", L"..\\Resources\\Texture\\Dungeon\\SteleIdle.png");
		Resources::Load<Texture>(L"SteleOpen", L"..\\Resources\\Texture\\Dungeon\\SteleOpen.png");

		Resources::Load<Texture>(L"Stage1DoorClose", L"..\\Resources\\Texture\\Dungeon\\close1F.png");
		Resources::Load<Texture>(L"Stage1DoorOpen", L"..\\Resources\\Texture\\Dungeon\\open1F.png");
		Resources::Load<Texture>(L"Stage1DoorClosing", L"..\\Resources\\Texture\\Dungeon\\Door1FClosing.png");
		Resources::Load<Texture>(L"Stage1DoorOpening", L"..\\Resources\\Texture\\Dungeon\\Door1FOpening.png");


		
		// dungeonTexture
		Resources::Load<Texture>(L"1FStage1Texture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\1FStage1.png");
		Resources::Load<Texture>(L"1FStage2Texture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\1FStage2.png");
		Resources::Load<Texture>(L"1FStage3Texture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\1FStage3.png");
		Resources::Load<Texture>(L"2FStage1Texture", L"..\\Resources\\Texture\\Scene_Dungeon2F\\EntranceBossroom.png");
		Resources::Load<Texture>(L"2FStage2Texture", L"..\\Resources\\Texture\\Scene_Dungeon2F\\Bossroom.png");

		// sound
		Resources::Load<AudioClip>(L"title", L"..\\Resources\\Sound\\background\\title.ogg");
		Resources::Load<AudioClip>(L"Town", L"..\\Resources\\Sound\\background\\0.Town.ogg");
		Resources::Load<AudioClip>(L"JailField", L"..\\Resources\\Sound\\background\\1.JailField.ogg");
		Resources::Load<AudioClip>(L"JailBoss", L"..\\Resources\\Sound\\background\\1.JailBoss.ogg");

		Resources::Load<AudioClip>(L"stoneDoor", L"..\\Resources\\Sound\\etc\\stoneDoor.ogg");
		Resources::Load<AudioClip>(L"OpenInventory", L"..\\Resources\\Sound\\etc\\OpenInventory.ogg");

		Resources::Load<AudioClip>(L"RifleFire", L"..\\Resources\\Sound\\boss\\RifleFire.ogg");
		Resources::Load<AudioClip>(L"jumperLaserFire", L"..\\Resources\\Sound\\boss\\jumperLaserFire.ogg");

		Resources::Load<AudioClip>(L"BansheeAttack_pre", L"..\\Resources\\Sound\\monster\\BansheeAttack_pre.ogg");
		Resources::Load<AudioClip>(L"Hit_Monster", L"..\\Resources\\Sound\\monster\\Hit_Monster.ogg");
		Resources::Load<AudioClip>(L"LilithAttack", L"..\\Resources\\Sound\\monster\\LilithAttack.ogg");
		Resources::Load<AudioClip>(L"MonsterDie", L"..\\Resources\\Sound\\monster\\MonsterDie.ogg");
		Resources::Load<AudioClip>(L"SpawnMonster", L"..\\Resources\\Sound\\monster\\SpawnMonster.ogg");
		Resources::Load<AudioClip>(L"swish", L"..\\Resources\\Sound\\player\\swish-1.ogg");



		Resources::Load<AudioClip>(L"Jumping", L"..\\Resources\\Sound\\player\\Jumping.ogg");
		Resources::Load<AudioClip>(L"dash", L"..\\Resources\\Sound\\player\\dash.ogg");
		Resources::Load<AudioClip>(L"Hit_Player", L"..\\Resources\\Sound\\player\\Hit_Player.ogg");
		Resources::Load<AudioClip>(L"swing", L"..\\Resources\\Sound\\player\\swing.ogg");
		Resources::Load<AudioClip>(L"crossbow", L"..\\Resources\\Sound\\player\\crossbow.ogg");
		Resources::Load<AudioClip>(L"Hit_Monster", L"..\\Resources\\Sound\\player\\Hit_Monster.ogg");
	}
	void LoadMaterial()
	{
#pragma region Sample
		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"SampleTexture");
		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		// smileTexture
		{
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"SampleMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"PaintTexture");
			material->SetTexture(texture);
			material->SetShader(shader);
			//material->SetRenderingMode(graphics::eRenderingMode::Transparent);
			Resources::Insert<Material>(L"SampleMaterial2", material);
		}
#pragma endregion
#pragma region Basic (NoneTexture)
		// animation Material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"AnimationShader");
			material->SetShader(shader);
			//material->SetRenderingMode(graphics::eRenderingMode::Transparent);
			Resources::Insert<Material>(L"AnimationMaterial", material);
		}
		// None material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"NoneMaterial", material);
		}
		// weapon material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"MeleeWeaponMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"LongSwordIconMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"CrossBowIconMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"QuarterStaffIconMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"RotateAnimationShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"RangeWeaponMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"RotateAnimationShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"RotateAnimationMaterial", material);
		}
		// projectile material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ProjectileMaterial", material);
		}
		// debug material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"DebugShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"DebugMaterial", material);
		}
		// Grid
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"GridShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"GridMaterial", material);
		}
		// particle
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"circleParticleTexture");
			shader = Resources::Find<Shader>(L"ParticleShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			//material->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"ParticleMaterial", material);
		}
		// None projectile material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"InvisibleShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"InvisibleProjectileMaterial", material);
		}
		// Animation projectile material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"AnimationShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"AnimationProjectileMaterial", material);
		}
		// Texture projectile material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"TextureProjectileMaterial", material);
		}
#pragma endregion
#pragma region HUD Material		
		// Basic Cursor
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"BasicCursorTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"CursorMaterial", material);
		}

		// life Panel
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"PlayerLifePanelTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"PlayerLifePanelMaterial", material);
		}
		// life Bar
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"PlayerLifeBarTexture");
			shader = Resources::Find<Shader>(L"LifeBarShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"PlayerLifeBarMaterial", material);
		}
		// Monster life Bar
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"PlayerLifeBarTexture");
			shader = Resources::Find<Shader>(L"LifeBarShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"CreatureLifeBarMaterial", material);
		}
		// dashcount panel
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"DashPanelTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"DashPanelMaterial", material);
		}
		// dashcount bar
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"DashActivateTexture");
			shader = Resources::Find<Shader>(L"DashCountShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"DashActivateMaterial", material);
		}
		// weapon panel
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"Armour1Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"Armour1Material", material);
		}
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"Armour2Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"Armour2Material", material);
		}
		// weapon Icon Material
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"WeaponIconAMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"WeaponIconBMaterial", material);
		}
		// armourPanel Icon
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ArmourIconAMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ArmourIconBMaterial", material);
		}

#pragma endregion
#pragma region Inventory Material
		// InventorySelect : NoneTexture Material | Sprite Shader
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"InventoryPanelMaterial", material);
		}
		// WeaponSlot 1~2 : NoneTexture Material | Sprite Shader
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"WeaponSlot1Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"WeaponSlot2Material", material);
		}
		// shiledSlot 1~2 : NoneTexture Material | Sprite Shader
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ShiledSlot1Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ShiledSlot2Material", material);
		}
		// accessorySlot 1~4 : NoneTexture Material | Sprite Shader
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"AccessorySlot1Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"AccessorySlot2Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"AccessorySlot3Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"AccessorySlot4Material", material);
		}
		// ItemSlot 1~ 15 : NoneTexture Material | Sprite Shader
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot00Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot01Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot02Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot03Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot04Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot10Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot11Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot12Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot13Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot14Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot20Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot21Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot22Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot23Material", material);
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ItemSlot24Material", material);
		}
#pragma endregion
#pragma region Title Material
		// TitleBtn 
		{
			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"StartBtnMaterial", material);

			material = std::make_shared<Material>();
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetShader(shader);
			Resources::Insert<Material>(L"ExitBtnMaterial", material);
		}

		// TitleScene BG layer | Sprite Shader	
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"TitleBackgroundTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"TitleBackgroundMaterial", material);
		}
		// Front Layer, Back Layer, Small Clouds | MovingBG Shader
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"BackCloudTexture");
			shader = Resources::Find<Shader>(L"MovingBGShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"BackCloudMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"FrontCloudTexture");
			shader = Resources::Find<Shader>(L"MovingBGShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"FrontCloudMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"SmallCloudTexture");
			shader = Resources::Find<Shader>(L"MovingBGShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"SmallCloudMaterial", material);
		}

		// Title MainLogo & CopyRight | sprite shader
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"MainLogoTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"MainLogoMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"CopyrightTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"CopyrightMaterial", material);
		}
#pragma endregion
#pragma region Town material
		// sky
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"SkyTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"SkyMaterial", material);
		}
		// TownBG
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"TownBGTexture");
			shader = Resources::Find<Shader>(L"TilingLayerShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"TownBGMaterial", material);
		}
		// TownLayer
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"TownLayerDayTexture");
			shader = Resources::Find<Shader>(L"TilingLayerShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"TownLayerMaterial", material);
		}

		// SecondFloor
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"TownMapTileTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"TownMapTileMaterial", material);
		}
		// platform
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"SecondFloor_OneWayTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"SecondFloor_OneWayMaterial", material);
		}
#pragma endregion
#pragma region 1F material
		// platform
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"Platform1FTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"Platform1FMaterial", material);
		}
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"Platform1Fx4Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"Platform1Fx4Material", material);
		}
		// Sub BG
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"DungeonSubBGTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"DungeonSubBGMaterial", material);
		}
		// close door
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"Close1FTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"Close1FMaterial", material);
		}
		// open door
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"Open1FTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"Open1FMaterial", material);
		}
		// stage1
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"1FStage1Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"1FStage1Material", material);
		}

		//stage2
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"1FStage2Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"1FStage2Material", material);
		}

		//stage3
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"1FStage3Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"1FStage3Material", material);
		}
		
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"KeyFTexture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"IconMaterial", material);
		}
#pragma endregion
#pragma region 2F material
		// stage1
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"2FStage1Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"2FStage1Material", material);
		}
		// stage2
		{
			material = std::make_shared<Material>();
			texture = Resources::Find<Texture>(L"2FStage2Texture");
			shader = Resources::Find<Shader>(L"SpriteShader");
			material->SetTexture(texture);
			material->SetShader(shader);
			Resources::Insert<Material>(L"2FStage2Material", material);
		}
#pragma endregion
	}
	void Initialize()
	{
		LoadMesh();
		LoadBuffer();
		LoadShader();
		SetupState();
		LoadTexture();
		LoadMaterial();
	}
	void Render()
	{
		BindNoiseTexture();
		BindLights();
		for (Camera* camera : cameras)
		{
			if (nullptr == camera)
				continue;

			camera->Render();
		}

		cameras.clear();
		lights.clear();
	}
	void PushDebugMeshAttribute(DebugMesh mesh)
	{
		debugMeshs.push_back(mesh);
	}
	void BindNoiseTexture()
	{
		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"Noise01");

		texture->BindShaderResource(eShaderStage::VS, 15);
		texture->BindShaderResource(eShaderStage::HS, 15);
		texture->BindShaderResource(eShaderStage::DS, 15);
		texture->BindShaderResource(eShaderStage::GS, 15);
		texture->BindShaderResource(eShaderStage::PS, 15);
		texture->BindShaderResource(eShaderStage::CS, 15);

		ConstantBuffer* noiseCB = constantBuffer[(UINT)eCBType::Noise];
		NoiseCB noiseData = {};
		noiseData.NoiseSize.x = (float)texture->GetWidth();
		noiseData.NoiseSize.y = (float)texture->GetHeight();

		noiseCB->SetData(&noiseData);
		noiseCB->Bind(eShaderStage::VS);
		noiseCB->Bind(eShaderStage::GS);
		noiseCB->Bind(eShaderStage::PS);
		noiseCB->Bind(eShaderStage::CS);
	}
	void BindLights()
	{
		std::vector<LightAttribute> lightsAttributes = {};
		for (Light* light : lights)
		{
			LightAttribute attribute = light->GetAttribute();
			lightsAttributes.push_back(attribute);
		}

		lightsBuffer->SetData(lightsAttributes.data(), (UINT)lightsAttributes.size());
		lightsBuffer->BindSRV(eShaderStage::VS, 13);
		lightsBuffer->BindSRV(eShaderStage::PS, 13);
	}
	void Release()
	{
		for (ConstantBuffer* buff : constantBuffer)
		{
			if (buff == nullptr)
				continue;

			delete buff;
			buff = nullptr;
		}

		delete lightsBuffer;
		lightsBuffer = nullptr;

	}
}