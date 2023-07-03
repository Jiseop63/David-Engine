#pragma
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"

#include "daResources.h"
#include "daMesh.h"
#include "daMaterial.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

#include "daUIObject.h"
#include "daUIScript.h"

namespace da::objects
{
	template <typename T>
	static T* InstantiateObject(Scene* scene, enums::eLayerType layer, const std::wstring& material)
	{
		T* obj = new T();										// 이때 Transform 추가됨
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);
		
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));

		obj->Initialize();
		return obj;
	}

	// 아직 손을 봐야할것 같음
	template <typename T>
	static T* InstantiateUIObject(Scene* scene)
	{
		T* obj = new T();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::UI);
		myLayer.AddGameObject(obj);

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"UIMaterial"));
				
		obj->Initialize();
		return obj;
	}

	template <typename T>
	static T* InstantiateButtonObject(Scene* scene, const std::wstring& material, const std::wstring& first, const std::wstring& second)
	{
		T* obj = new T();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::UI);
		myLayer.AddGameObject(obj);
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh( Resources::Find<Mesh>(L"RectMesh") );
		meshRenderer->SetMaterial( Resources::Find<Material>(material) );
		UIScript* uiScript = obj->AddComponent<UIScript>();
		obj->Initialize();

		uiScript->SetUITextures( Resources::Find<graphics::Texture>(first), Resources::Find<graphics::Texture>(second) );
		return obj;
	}


#pragma region Camera Objects
	static CameraObject* InstantiateMainCamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject(); 
		cameraObj->Initialize();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);
		cameraObj->AddComponent<CameraScript>();
		Camera* camera = cameraObj->GetCameraComponent();
		camera->TurnLayerMask(enums::eLayerType::UI, false);

		return cameraObj;
	}
	static CameraObject* InstantiateUICamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject();
		cameraObj->Initialize();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);
		Camera* camera = cameraObj->GetCameraComponent();

		camera->DisableLayerMask();
		camera->TurnLayerMask(enums::eLayerType::UI);
		return cameraObj;
	}
#pragma endregion
}