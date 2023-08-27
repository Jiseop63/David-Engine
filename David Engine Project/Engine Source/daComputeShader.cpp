#include "daComputeShader.h"
#include "daGraphicDevice Dx11.h"

namespace da::graphics
{
	ComputeShader::ComputeShader()
		: Resource(enums::eResourceType::ComputeShader)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)
	{
		mThreadGroupCountX = 32;
		mThreadGroupCountY = 32;
		mThreadGroupCountZ = 1;
	}
	ComputeShader::ComputeShader(int x, int y, int z)
		: Resource(enums::eResourceType::ComputeShader)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)
	{
		mThreadGroupCountX = x;
		mThreadGroupCountY = y;
		mThreadGroupCountZ = z;
	}
	ComputeShader::~ComputeShader()
	{
	}

	bool ComputeShader::Create(const std::wstring& name, const std::string& methodName)
	{
		std::filesystem::path shaderPath
			= std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader Source\\";
		
		std::filesystem::path fullPath(shaderPath.c_str());
		fullPath += name;

		ID3DBlob* errorBlob = nullptr;
		graphics::GetDevice()->CompileFromFile(fullPath, methodName, "cs_5_0", mCSBlob.GetAddressOf());
		graphics::GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer()
			, mCSBlob->GetBufferSize(), mCS.GetAddressOf());

		return true;
	}
	void ComputeShader::OnExcute()
	{
		Binds();
		GetDevice()->BindComputeShader(mCS.Get());
		GetDevice()->Dispatch(mGroupX, mGroupY, mGroupZ);
		Clear();
	}
}
