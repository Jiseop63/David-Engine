#pragma once
#include "daResource.h"
#include "daGraphicDevice Dx11.h"

namespace da
{
	using namespace graphics;

	class Mesh : public Resource
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVertexBuffer(void* data, UINT count);
		bool CreateIndexBuffer(void* data, UINT count);

		void BindBuffer();
		void Render();

		UINT GetIndexCount() { return mIndexCount; }


	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		D3D11_BUFFER_DESC mVBDesc;
		D3D11_BUFFER_DESC mIBDesc;
		UINT mIndexCount;
	};
}