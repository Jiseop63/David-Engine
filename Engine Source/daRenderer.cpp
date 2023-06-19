#include "daRenderer.h"

namespace renderer
{
	Vertex vertexes[4] = {};
	ID3D11InputLayout* triangleLayout = nullptr;
	ID3D11Buffer* triangleBuffer = nullptr;
	ID3D11Buffer* triangleIndexBuffer = nullptr;
	ID3D11Buffer* triangleConstantBuffer = nullptr;
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* triangleVSBlob = nullptr;
	ID3D11VertexShader* triangleVSShader = nullptr;
	ID3DBlob* trianglePSBlob = nullptr;
	ID3D11PixelShader* trianglePSShader = nullptr;

	void SetupState()
	{

	}
	void LoadBuffer()
	{
		D3D11_BUFFER_DESC triangleDesc = {};
		triangleDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		triangleDesc.ByteWidth = sizeof(Vertex) * 4;
		triangleDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		triangleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA triangleData = {};
		triangleData.pSysMem = vertexes;
		da::graphics::GetDevice()->CreateBuffer(&triangleBuffer, &triangleDesc, &triangleData);

		
		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		D3D11_BUFFER_DESC triangleIndexDesc = {};
		triangleIndexDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		triangleIndexDesc.ByteWidth = sizeof(UINT) * indexes.size();
		triangleIndexDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		triangleIndexDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA triangleIndexData = {};
		triangleIndexData.pSysMem = indexes.data();
		da::graphics::GetDevice()->CreateBuffer(&triangleIndexBuffer, &triangleIndexDesc, &triangleIndexData);


		D3D11_BUFFER_DESC triangleConstantDesc = {};
		triangleConstantDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		triangleConstantDesc.ByteWidth = sizeof(Vector4);
		triangleConstantDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		triangleConstantDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		da::graphics::GetDevice()->CreateBuffer(&triangleConstantBuffer, &triangleConstantDesc, nullptr);


		Vector4 pos(0.3f, 0.0f, 0.0f, 1.0f);
		da::graphics::GetDevice()->SetConstantBuffer(triangleConstantBuffer, &pos, sizeof(Vector4));
		da::graphics::GetDevice()->BindConstantBuffer(eShaderStage::VS, eCBType::Transform, triangleConstantBuffer);
	}
	void LoadShader()
	{
		da::graphics::GetDevice()->CreateShader();
	}
	void Initialize()
	{
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		SetupState();
		LoadBuffer();
		LoadShader();
	}

	void Release()
	{
		if (triangleLayout != nullptr)
			triangleLayout->Release();

		if (triangleBuffer != nullptr)
			triangleBuffer->Release();

		if (triangleIndexBuffer != nullptr)
			triangleIndexBuffer->Release();

		if (triangleConstantBuffer != nullptr)
			triangleConstantBuffer->Release();

		if (errorBlob != nullptr)
			errorBlob->Release();

		if (triangleVSBlob != nullptr)
			triangleVSBlob->Release();

		if (triangleVSShader != nullptr)
			triangleVSShader->Release();

		if (trianglePSBlob != nullptr)
			trianglePSBlob->Release();

		if (trianglePSShader != nullptr)
			trianglePSShader->Release();
	}

}