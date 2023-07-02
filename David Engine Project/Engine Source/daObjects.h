#pragma
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"

#include "daResources.h"

#include "daGameObject.h"

#include "daMeshRenderer.h"
#include "daMesh.h"
#include "daMaterial.h"

namespace da::objects
{
	template <typename T>
	static T* InstantiateObject(Scene* scene, enums::eLayerType layer, const std::wstring& material)
	{
		T* obj = new T();										// ÀÌ¶§ Transform Ãß°¡µÊ
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);
		GameObject* gameObj = dynamic_cast<GameObject*>(obj);
		if (nullptr != gameObj)
		{
			gameObj->Initialize();
		}
		MeshRenderer* sr = gameObj->AddComponent<MeshRenderer>();
		sr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		sr->SetMaterial(Resources::Find<Material>(material));

		return obj;
	}
}