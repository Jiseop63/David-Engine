#pragma once
#include "daComponent.h"
#include "daMesh.h"
#include "daMaterial.h"
#include "daTexture.h"

namespace da
{
	class SubMeshRenderer : public Component
	{
	public:
		SubMeshRenderer();
		virtual ~SubMeshRenderer();

		virtual void Render() override;

		void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
		void SetMaterial(std::shared_ptr<Material> material) { mMaterial = material; }
		std::shared_ptr<Material> GetMaterial() { return mMaterial; }

	public:
		void ChangeTexture(std::shared_ptr<graphics::Texture> texture) { mMaterial->SetTexture(texture); }

	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;
	};
}