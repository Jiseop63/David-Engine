#include "daConstantBuffer.h"
#include "daGraphicDevice Dx11.h"

namespace da::graphics
{
	ConstantBuffer::ConstantBuffer(const eCBType type)
		: GPUBuffer()
		, mCBType(type)
	{
	}
	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(UINT size)
	{
		Desc.ByteWidth = size;
		Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		Desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		da::graphics::GetDevice()->CreateBuffer(Buffer.GetAddressOf(), &Desc, nullptr);

		return false;
	}
	void ConstantBuffer::SetData(void* data)
	{
		da::graphics::GetDevice()->SetConstantBuffer(Buffer.Get(), data, Desc.ByteWidth);
	}
	void ConstantBuffer::Bind(eShaderStage stage)
	{
		da::graphics::GetDevice()->BindConstantBuffer(stage, mCBType, Buffer.Get());
	}
}
