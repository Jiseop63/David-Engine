#pragma once
#include "daResource.h"
#include "daTexture.h"
#include "daShader.h"

namespace da::graphics
{
	class Material : public Resource
	{
	public:
		Material();
		~Material();

		virtual HRESULT Load(const std::wstring& path) override;

		void Binds();

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		void SetTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }
		void Clear();
	private:
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture;
	};
}
