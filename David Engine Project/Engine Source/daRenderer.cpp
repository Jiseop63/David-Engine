#include "daRenderer.h"
#include "daResources.h"
#include "daMesh.h"
#include "daTexture.h"
#include "daShader.h"
#include "daMaterial.h"

using namespace da;
using namespace da::graphics;
using namespace da::math;

namespace renderer
{
	renderer::Vertex vertexes[4] = {};	
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates[(UINT)eBSType::End] = {};
	da::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
	std::vector<da::Camera*> cameras = {};

	void LoadMesh()
	{		
#pragma region Create vertex & index buffer

		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"RectMesh", mesh);
		mesh->CreateVertexBuffer(vertexes, 4);

		std::vector<UINT> indexes = {};
		indexes.push_back(0); indexes.push_back(1); indexes.push_back(2);
		indexes.push_back(0); indexes.push_back(2); indexes.push_back(3);

		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
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
		// fade
		// afterimage
		// blink
#pragma endregion
	}
	void LoadResource()
	{
#pragma region Create Shaders

		// Default Shaders
		std::shared_ptr<Shader> triangleShader = std::make_shared<Shader>();
		{
			triangleShader->Create(eShaderStage::VS, L"TriangleShader.hlsl", "mainVS");
			triangleShader->Create(eShaderStage::PS, L"TriangleShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"TriangleShader", triangleShader);
		}
		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		{
			spriteShader->Create(eShaderStage::VS, L"SpriteShader.hlsl", "mainVS");
			spriteShader->Create(eShaderStage::PS, L"SpriteShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"SpriteShader", spriteShader);
		}
		// TitleBackground Shader
		std::shared_ptr<Shader> MovingBGShader = std::make_shared<Shader>();
		{
			MovingBGShader->Create(eShaderStage::VS, L"MovingBGShader.hlsl", "mainVS");
			MovingBGShader->Create(eShaderStage::PS, L"MovingBGShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"MovingBGShader", MovingBGShader);
		}
#pragma endregion

#pragma region Title :: Load Texture & Create Material

		// NoneTexture Material | Sprite Shader
		{
			std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
			uiMaterial->SetShader(spriteShader);
			uiMaterial->SetRenderingMode(eRenderingMode::Transparent);
			std::shared_ptr<Material> uiMaterial2 = std::make_shared<Material>();
			uiMaterial->SetShader(spriteShader);
			uiMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert<Material>(L"StartBtnMaterial", uiMaterial);
			Resources::Insert<Material>(L"ExitBtnMaterial", uiMaterial2);
		}

		// TitleScene BG layer | Sprite Shader	
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleBackgroundTexture", L"..\\Resources\\Texture\\Scene_Title\\TitleBackground.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
			Resources::Insert<Material>(L"TitleBackgroundMaterial", spriteMaterial);
		}
		// Front Layer, Back Layer, Small Clouds | MovingBG Shader
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BackCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\BackCloud.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(MovingBGShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"BackCloudMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"FrontCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\FrontCloud.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(MovingBGShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"FrontCloudMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"SmallCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\smallCloud.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(MovingBGShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"SmallCloudMaterial", spriteMaterial);
		}

		// Title MainLogo & CopyRight | sprite shader
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MainLogoTexture", L"..\\Resources\\Texture\\Scene_Title\\MainLogo.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"MainLogoMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"CopyrightTexture", L"..\\Resources\\Texture\\Scene_Title\\Copyright.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"CopyrightMaterial", spriteMaterial);
		}

		// TitleScene Button Textures
		{
			Resources::Load<Texture>(L"PlayOffTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\PlayOff_Kor.png");
			Resources::Load<Texture>(L"PlayOnTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\PlayOn_Kor.png");
			
			Resources::Load<Texture>(L"ExitOffTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\ExitOff_Kor.png");
			Resources::Load<Texture>(L"ExitOnTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\ExitOn_Kor.png");
		}
#pragma endregion

#pragma region UI
		// player life
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerLifePanelTexture", L"..\\Resources\\Texture\\UIs\\InGame\\Life\\PlayerLifePanel.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"PlayerLifePanelMaterial", spriteMaterial);
		}
		// dash count
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DashPanelTexture", L"..\\Resources\\Texture\\UIs\\InGame\\DashCount\\DashPanel.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"DashPanelMaterial", spriteMaterial);
		}
		// weapon panel
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"WeaponBaseTexture", L"..\\Resources\\Texture\\UIs\\InGame\\Weapon\\Base.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
			Resources::Insert<Material>(L"WeaponBaseMaterial", spriteMaterial);
		}
#pragma endregion

#pragma region Town :: Load Texture & Create material
		// sky
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"SkyTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\Sky_Day.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
			Resources::Insert<Material>(L"SkyMaterial", spriteMaterial);
		}
		// TownBG
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TownBGTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\TownBG_Day.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"TownBGMaterial", spriteMaterial);
		}
		// TownLayer
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TownLayerDayTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\TownLayer_Day.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Cutout);
			Resources::Insert<Material>(L"TownLayerMaterial", spriteMaterial);
		}
#pragma endregion

#pragma region 1F :: Load Texture & Create material
		// Temp dungeon
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TempDungeonTexture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\tempDungeon.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
			Resources::Insert<Material>(L"TempDungeonMaterial", spriteMaterial);
		}

#pragma endregion
#pragma region 2F :: Load Texture & Create material
		// Temp dungeon
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"2FTexture", L"..\\Resources\\Texture\\Scene_Dungeon2F\\2F.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
			Resources::Insert<Material>(L"2FMaterial", spriteMaterial);
		}

#pragma endregion
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



#pragma endregion
#pragma region Sampler State

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSamplers(eShaderStage::PS, 0, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSamplers(eShaderStage::PS, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		// Soild Back	: 뒷면을 렌더링하지 않음
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());
		// Soild Front	: 앞면을 렌더링하지 않음
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());
		// Solid None	: 모든 면을 렌더링함
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());
		// Wireframe	: 뼈대만 렌더링함
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
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
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::Greater].GetAddressOf());
		// No Write <- 덮어쓰기 안함
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());
		// None 
		depthStencilDesc.DepthEnable = false;
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
	void Initialize()
	{
		
		LoadMesh();
		LoadBuffer();
		LoadResource();
		SetupState();
	}

	void Render()
	{
		for (Camera* camera : cameras)
		{
			if (nullptr == camera)
				continue;

			camera->Render();
		}

		cameras.clear();
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
	}
}