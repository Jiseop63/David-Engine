#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"
#include "daMesh.h"
#include "daShader.h"

using namespace da::math;
namespace renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[];
	extern ID3D11Buffer* triangleConstantBuffer;

	extern da::Mesh* mesh;
	extern da::Shader* shader;


	void Initialize();
	void Release();
}
