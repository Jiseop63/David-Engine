#include "daMaterial.h"

namespace da::graphics
{
	Material::Material()
		: Resource(da::enums::eResourceType::Material)
		, mTexture(nullptr)
		, mShader(nullptr)
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
		mTexture->BindShader(eShaderStage::PS, 0);
		mShader->Binds();
	}
	void Material::Clear()
	{
		mTexture->Clear();
	}
}
