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

	template <typename T>
	static T* InstantiateCreature(Scene* scene, enums::eLayerType layer, const std::wstring& material)
	{
		T* obj = new T();
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		obj->AddComponent<Rigidbody>();
		obj->AddComponent<Collider2D>();
		obj->Initialize();
		return obj;
	}
	
#pragma endregion
#pragma region Quick Init objects
	
	template <typename T>
	static T* InstantiateUIObject(Scene* scene, enums::eLayerType layer, const std::wstring& material)
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

	static GameObject* InstantiatePlayer(Scene* scene, const std::wstring& material)
	{
		GameObject* obj = new GameObject();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Playable);
		myLayer.AddGameObject(obj);

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		obj->AddComponent<Rigidbody>();
		obj->AddComponent<Collider2D>();
		obj->AddComponent<Animator>();
		obj->AddComponent<PlayerScript>();
		obj->Initialize();
		return obj;
	}

	static GameObject* InstantiateGridObject(Scene* scene, CameraObject* cameraObject)
	{
		GameObject* obj = new GameObject();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
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
		cameraObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -6.750f));
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);
		Camera* camera = cameraObj->AddComponent<Camera>();
		cameraObj->SetCameraComponent(camera);
		CameraScript* script = cameraObj->AddComponent<CameraScript>();
		script->SetCamera(camera);
				
		// 서순 중요함
		cameraObj->Initialize();
		camera->TurnLayerMask(enums::eLayerType::UI, false);
		camera->TurnLayerMask(enums::eLayerType::Background, false);
		camera->TurnLayerMask(enums::eLayerType::Layer, false);
		return cameraObj;
	}
	static CameraObject* InstantiateUICamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject();
		cameraObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -6.750f));
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);
		Camera* camera = cameraObj->AddComponent<Camera>();
		cameraObj->SetCameraComponent(camera);
		cameraObj->Initialize();

		// 서순 중요함
		camera->DisableLayerMask();
		camera->TurnLayerMask(enums::eLayerType::UI);
		return cameraObj;
	}

	static CameraObject* InstantiateSubCamera(Scene* scene)
	{
		CameraObject* cameraObj = new CameraObject();
		cameraObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -6.750f));
		Layer& myLayer = scene->GetLayer(enums::eLayerType::None);
		myLayer.AddGameObject(cameraObj);
		Camera* camera = cameraObj->AddComponent<Camera>();
		cameraObj->SetCameraComponent(camera);
		cameraObj->AddComponent<SubCameraScript>();
		cameraObj->Initialize();

		// 서순 중요함
		camera->DisableLayerMask();
		camera->TurnLayerMask(enums::eLayerType::Background);
		camera->TurnLayerMask(enums::eLayerType::Layer);
		return cameraObj;
	}
#pragma endregion

}