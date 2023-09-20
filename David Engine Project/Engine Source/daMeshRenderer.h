#pragma once
#include "daComponent.h"
#include "daMesh.h"
#include "daMaterial.h"
#include "daTexture.h"

namespace da
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();
		virtual void Render() override;

		void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
		void SetMaterial(std::shared_ptr<Material> material) { mMaterial = material; }
		std::shared_ptr<Mesh> GetMesh() { return mMesh; }
		std::shared_ptr<Material> GetMaterial() { return mMaterial; }
	public:
		void ChangeMaterialTexture(std::shared_ptr<graphics::Texture> texture) { mMaterial->SetTexture(texture); }

		void SetReverse(bool isReverse) { mReverse = isReverse; }
		void BindReverseCB();
	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;
		bool mReverse;
	};
}