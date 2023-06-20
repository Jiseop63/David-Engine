#pragma once
#include "daGraphics.h"

namespace da::graphics
{
	class ConstantBuffer : public GPUBuffer
	{
	public:
		ConstantBuffer(const eCBType type);
		virtual ~ConstantBuffer();

		bool Create(UINT size);
		void SetData(void* data);
		void Bind(eShaderStage stage);

	private:
		const eCBType mType;
	};
}