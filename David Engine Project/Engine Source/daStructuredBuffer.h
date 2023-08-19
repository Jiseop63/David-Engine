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

		bool Create(UINT size, UINT stride, eViewType type, void* data);
		void SetData(void* data, UINT stride);
		void BindSRV(eShaderStage stage, UINT slot);
		void BindUAV(UINT slot);

		void Clear();

	public:
		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	mUAV;
		eViewType mViewType;
		UINT mSize;
		UINT mStride;
		UINT mSRVSlot;
		UINT mUAVSlot;
	};
}
