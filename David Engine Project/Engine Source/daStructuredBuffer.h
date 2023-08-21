#pragma once
#include "daEntity.h"
#include "daGraphics.h"

namespace da::graphics
{
	class StructuredBuffer : public GPUBuffer
	{
	public:
		StructuredBuffer();
		virtual ~StructuredBuffer();

		bool Create(UINT size, UINT stride, eSRVType type);
		void SetData(void* data, UINT stride);
		void Bind(eShaderStage stage, UINT slot);

		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		eSRVType mSRVType;
		UINT mSize;
		UINT mStride;
	};
}
