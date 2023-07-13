#include "daConstantBuffer.h"
#include "daGraphicDevice Dx11.h"

namespace da::graphics
{
	ConstantBuffer::ConstantBuffer(const eCBType type)
		: GPUBuffer()
		, mColliderType(type)
	{
	}
	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(UINT size)
	{
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		da::graphics::GetDevice()->CreateBuffer(Buffer.GetAddressOf(), &desc, nullptr);

		return false;
	}
	void ConstantBuffer::SetData(void* data)
	{
		da::graphics::GetDevice()->SetConstantBuffer(Buffer.Get(), data, desc.ByteWidth);
	}
	void ConstantBuffer::Bind(eShaderStage stage)
	{
		da::graphics::GetDevice()->BindConstantBuffer(stage, mColliderType, Buffer.Get());
	}
}
