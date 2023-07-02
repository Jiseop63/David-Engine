#pragma
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"

#include "daResources.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daMesh.h"
#include "daMaterial.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

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
			gameObj->Initialize();

		MeshRenderer* meshRenderer = gameObj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));

		return obj;
	}

	static CameraObject* InstantiateMainCamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject(); 
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None); 
		myLayer.AddGameObject(cameraObj);
		cameraObj->Initialize();
		cameraObj->AddComponent<CameraScript>();
		Camera* camera = cameraObj->GetCameraComponent();
		camera->TurnLayerMask(enums::eLayerType::UI, false);
		return cameraObj;
	}
	static CameraObject* InstantiateUICamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);
		cameraObj->Initialize();
		Camera* camera = cameraObj->GetCameraComponent();
		camera->DisableLayerMask();
		camera->TurnLayerMask(enums::eLayerType::UI);

		return cameraObj;
	}
}