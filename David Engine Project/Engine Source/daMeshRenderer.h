#pragma once
#include "daComponent.h"
#include "daMesh.h"
#include "daShader.h"

namespace da
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		Mesh* mMesh;
		Shader* mShader;
	};
}