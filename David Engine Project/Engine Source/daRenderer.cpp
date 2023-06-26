#include "daRenderer.h"
#include "daResources.h"
#include "daTexture.h"
#include "daMaterial.h"

using namespace da;
using namespace da::graphics;
using namespace da::math;

namespace renderer
{
	Vertex vertexes[4] = {};	
	ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[(UINT)eSamplerType::End] = {};
	
	
	void SetupState()
	{
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


		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSampler(&samplerDesc, samplerState[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 0, samplerState[(UINT)eSamplerType::Point].GetAddressOf());

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSampler(&samplerDesc, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 1, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
	}
	void LoadBuffer()
	{
		// Create Vertex & Index Buffer
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"RectMesh", mesh);
		mesh->CreateVertexBuffer(vertexes, 4);
				
		std::vector<UINT> indexes = {};
		indexes.push_back(0); indexes.push_back(1); indexes.push_back(2); 
		indexes.push_back(0); indexes.push_back(2); indexes.push_back(3);

		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

		// Create ConstantBuffer 
		constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffer[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));
	}
	void LoadResource()
	{
		std::shared_ptr<Shader> triangleShader = std::make_shared<Shader>();
		triangleShader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		triangleShader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");
		Resources::Insert<Shader>(L"TriangleShader", triangleShader);

		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		Resources::Insert<Shader>(L"SpriteShader", spriteShader);

		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"SmileTexture", L"..\\Resources\\Texture\\smileTexture.png");

		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->SetTexture(texture);
		spriteMaterial->SetShader(spriteShader);
		Resources::Insert<Material>(L"SpriteMaterial", spriteMaterial);

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleBackGroundTexture", L"..\\Resources\\Texture\\TitleBackGround.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"TitleBackGroundMaterial", spriteMaterial);
		}

		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"CloudATexture", L"..\\Resources\\Texture\\CloudA.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"CloudAMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"CloudBTexture", L"..\\Resources\\Texture\\CloudB.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"CloudBMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"CloudCTexture", L"..\\Resources\\Texture\\CloudC.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"CloudCMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleLogoTexture", L"..\\Resources\\Texture\\TitleLogo.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"TitleLogoMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"GotoSelectButtonTexture", L"..\\Resources\\Texture\\GotoSelectButton.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"GotoSelectButtonMaterial", spriteMaterial);
		}
	}
	void Initialize()
	{
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

		LoadBuffer();
		LoadResource();
		SetupState();

		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\smileTexture.png");

		texture->BindShader(eShaderStage::PS, 0);
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