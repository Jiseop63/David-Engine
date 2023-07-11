#pragma
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"

#include "daResources.h"
#include "daMesh.h"
#include "daMaterial.h"

#include "daObjectsFastIncludeHeader.h"

namespace da::objects
{
#pragma region Basic GameObjects Func
	
	template <typename T>
	static T* JustGameObject(const std::wstring& material)
	{
		T* obj = new T();
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));

		obj->Initialize();
		return obj;
	}
	template <typename T>
	static T* InstantiateObject(Scene* scene, enums::eLayerType layer)
	{
		T* obj = new T();
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);
		return obj;
	}
	template <typename T>
	static T* InstantiateGameObject(Scene* scene, enums::eLayerType layer, const std::wstring& material)
	{
		T* obj = new T();
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));

		obj->Initialize();
		return obj;
	}

#pragma endregion
#pragma region Quick Init objects
	
	template <typename T>
	static T* InstantiateUIObject(Scene* scene, enums::eLayerType layer, const std::wstring& material)
	{
		T* obj = new T();										// ÀÌ¶§ Transform Ãß°¡µÊ
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));

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
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		ButtonScript* uiScript = obj->AddComponent<ButtonScript>();
		obj->Initialize();

		uiScript->SetUITextures(Resources::Find<graphics::Texture>(first), Resources::Find<graphics::Texture>(second));
		return obj;
	}

	template <typename T>
	static T* InstantiateMultiTextureUI(Scene* scene, const std::wstring& material, const std::wstring& first, const std::wstring& second)
	{
		T* obj = new T();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::UI);
		myLayer.AddGameObject(obj);
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		UIScript* uiScript = obj->AddComponent<UIScript>();
		obj->Initialize();
		uiScript->SetUITextures(Resources::Find<graphics::Texture>(first), Resources::Find<graphics::Texture>(second));
		return obj;
	}

	
	static GameObject* InstantiateGridObject(Scene* scene, CameraObject* cameraObject)
	{
		GameObject* obj = new GameObject();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Grid);
		myLayer.AddGameObject(obj);
		GridScript* gridScript = obj->AddComponent<GridScript>();
		gridScript->SetCamera(cameraObject->GetCameraComponent());

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh( Resources::Find<Mesh>(L"RectMesh") );
		meshRenderer->SetMaterial( Resources::Find<Material>(L"GridMaterial") );
		obj->Initialize();
		return obj;
	}
#pragma endregion 
#pragma region Camera objects
	static CameraObject* InstantiateMainCamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject(); 
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);
		Camera* camera = cameraObj->GetCameraComponent();
		CameraScript* script = cameraObj->AddComponent<CameraScript>();
		cameraObj->Initialize();
		
		camera->TurnLayerMask(enums::eLayerType::UI, false);
		script->SetCamera(camera);
		return cameraObj;
	}
	static CameraObject* InstantiateUICamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);

		Camera* camera = cameraObj->GetCameraComponent();
		camera->DisableLayerMask();
		camera->TurnLayerMask(enums::eLayerType::UI);
		cameraObj->Initialize();
		return cameraObj;
	}
#pragma endregion

}