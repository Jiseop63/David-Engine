#include "daMaterial.h"

namespace da::graphics
{
	Material::Material()
		: Resource(da::enums::eResourceType::Material)
		, mTexture(nullptr)
		, mShader(nullptr)
		, mRenderingMode(eRenderingMode::Cutout)
	{
	}
	Material::~Material()
	{
	}

	HRESULT Material::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	void Material::Binds()
	{
		if (mTexture)
			mTexture->BindShaderResource(eShaderStage::PS, 0);

		if (mShader)
			mShader->Binds();
	}
	void Material::Clear()
	{
		mTexture->Clear();
	}

}
