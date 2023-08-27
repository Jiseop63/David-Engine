#include "daPaintShader.h"
#include "daTexture.h"

namespace da::graphics
{
	void PaintShader::Binds()
	{
		mTarget->BindUnorderedAccessViews(0);
		mGroupX = mTarget->GetWidth() / mThreadGroupCountX + 1;
		mGroupY = mTarget->GetHeight() / mThreadGroupCountY + 1;
		mGroupZ = 1;
	}

	void PaintShader::Clear()
	{
		mTarget->ClearUnorderedAccessViews(0);
	}
}
