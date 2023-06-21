#pragma once
#include "daComponent.h"
#include "daMesh.h"
#include "daMaterial.h"

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

		void SetMesh(Mesh* mesh) { mMesh = mesh; }
		void SetMaterial(Material* material) { mMaterial = material; }
	private:
		Mesh* mMesh;
		Material* mMaterial;
	};
}