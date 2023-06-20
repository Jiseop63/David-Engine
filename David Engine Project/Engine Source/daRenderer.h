#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"
#include "daMesh.h"
#include "daShader.h"
#include "daConstantBuffer.h"

using namespace da::math;
namespace renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[];

	extern da::Mesh* mesh;
	extern da::Shader* shader;
	extern da::ConstantBuffer* constantBuffer;

	void Initialize();
	void Release();
}
