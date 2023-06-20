#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"
#include "daMesh.h"
#include "daShader.h"
#include "daConstantBuffer.h"

using namespace da;
using namespace da::graphics;
using namespace da::math;
namespace renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[];

	extern Mesh* mesh;
	extern Shader* shader;
	extern ConstantBuffer* constantBuffer;

	void Initialize();
	void Release();
}
