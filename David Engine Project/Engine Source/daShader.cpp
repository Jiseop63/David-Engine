#include "daShader.h"
#include "daRenderer.h"

namespace da
{
	Shader::Shader()
		: Resource(enums::eResourceType::Shader)
		, mInputLayout(nullptr)
		, mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, mRSType(eRSType::SolidBack)
		, mDSType(eDSType::Less)
		, mBSType(eBSType::AlphaBlend)
	{
	}
	Shader::~Shader()
	{
		mInputLayout->Release();
	}
	HRESULT Shader::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	bool Shader::Create(const eShaderStage stage, const std::wstring& fileName, const std::string& funcName)
	{
		std::filesystem::path shaderPath
			= std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader Source\\";


		std::filesystem::path completePath(shaderPath.c_str());
		completePath += fileName;
		
		if (eShaderStage::VS == stage)
		{
			GetDevice()->CompileFromFile(completePath, funcName, "vs_5_0", mVSBlob.GetAddressOf());
			GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), mVS.GetAddressOf());
		}
		if (eShaderStage::GS == stage)
		{
			GetDevice()->CompileFromFile(completePath, funcName, "gs_5_0", mGSBlob.GetAddressOf());
			GetDevice()->CreateGeometryShader(mGSBlob->GetBufferPointer(), mGSBlob->GetBufferSize(), mGS.GetAddressOf());
		}
		if (eShaderStage::PS == stage)
		{
			GetDevice()->CompileFromFile(completePath, funcName, "ps_5_0", mPSBlob.GetAddressOf());
			GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), mPS.GetAddressOf());
		}
		return true;
	}
	void Shader::Binds()
	{
		GetDevice()->BindPrimitiveTopology(mTopology);
		GetDevice()->BindInputLayout(mInputLayout);
		GetDevice()->BindVertexShader(mVS.Get());
		GetDevice()->BindGeometryShader(mGS.Get());
		GetDevice()->BindPixelShader(mPS.Get());

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rsState = renderer::RasterizerStates[(UINT)mRSType];
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState = renderer::DepthStencilStates[(UINT)mDSType];
		Microsoft::WRL::ComPtr<ID3D11BlendState> bsState = renderer::BlendStates[(UINT)mBSType];

		GetDevice()->BindRasterizerState(rsState.Get());
		GetDevice()->BindDepthStencilState(dsState.Get());
		GetDevice()->BindBlendState(bsState.Get());
	}
}