#pragma
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"

#include "daResources.h"
#include "daMesh.h"
#include "daMaterial.h"

#include "daObjectsFastIncludeHeader.h"

#define PLAYER_EFFECT_POOL 20
#define WEAPON_EFFECT_POOL 5
#define PLAYER_PROJECTILE_POOL 30

namespace da::objects
{
#pragma region Basic GameObjects Func
	
	// 위치값을 잡아주는 Parent 구조로 사용
	template <typename T>
	static T* InstantiateObject(Scene* scene, enums::eLayerType layer)
	{
		T* obj = new T();
		GameObject* gameObject = dynamic_cast<GameObject*>(obj);
		gameObject->SetLayerType(layer);
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);
		return obj;
	}
	// 기본적으로 사용
	template <typename T>
	static T* InstantiateGameObject(Scene* scene, enums::eLayerType layer, const std::wstring& material)
	{
		T* obj = new T();
		GameObject* gameObject = dynamic_cast<GameObject*>(obj);
		gameObject->SetLayerType(layer);
		Layer& myLayer = scene->GetLayer(layer);
		myLayer.AddGameObject(obj);

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));		
		obj->Initialize();
		return obj;
	}
	template <typename T>
	static T* InstantiateCommonObject(Scene* scene, enums::eLayerType layer, const std::wstring& material)
	{
		T* obj = new T();
		GameObject* gameObject = dynamic_cast<GameObject*>(obj);
		gameObject->SetLayerType(layer);
		gameObject->SetCommonObject(true);
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
	
	// UI에 쓰임
	template <typename T>
	static T* InstantiateButtonObject(Scene* scene, const std::wstring& material, const std::wstring& first, const std::wstring& second)
	{
		T* obj = new T();
		GameObject* gameObject = dynamic_cast<GameObject*>(obj);
		gameObject->SetLayerType(enums::eLayerType::UI);
		Layer& myLayer = scene->GetLayer(enums::eLayerType::UI);
		myLayer.AddGameObject(obj);
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		ButtonScript* uiScript = obj->AddComponent<ButtonScript>();
		obj->Initialize();

		uiScript->SetSlotTextures(Resources::Find<graphics::Texture>(first), Resources::Find<graphics::Texture>(second));
		return obj;
	}
	template <typename T>
	static T* InstantiateMultiTextureUI(Scene* scene, const std::wstring& material, const std::wstring& first, const std::wstring& second)
	{
		T* obj = new T();
		GameObject* gameObject = dynamic_cast<GameObject*>(obj);
		gameObject->SetLayerType(enums::eLayerType::UI);
		Layer& myLayer = scene->GetLayer(enums::eLayerType::UI);
		myLayer.AddGameObject(obj);
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		UIScript* uiScript = obj->AddComponent<UIScript>();
		obj->Initialize();
		uiScript->SetSlotTextures(Resources::Find<graphics::Texture>(first), Resources::Find<graphics::Texture>(second));
		return obj;
	}


	static GameObject* InstantiatePlayer(Scene* scene)
	{
		// 플레이어 세팅
		GameObject* player = new GameObject();
		player->SetLayerType(enums::eLayerType::Playable);
		player->SetCommonObject(true);
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Playable);
		myLayer.AddGameObject(player);

		MeshRenderer* meshRenderer = player->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"AnimationMaterial"));
		Light* playerLight = player->AddComponent<Light>();
		playerLight->SetRadius(2.5f);
		playerLight->SetLightType(enums::eLightType::Point);
		playerLight->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));
		PlayerScript* playerScript = player->AddComponent<PlayerScript>();
		// 플레이어 이펙트 세팅
		for (int index = 0; index < PLAYER_EFFECT_POOL; index++)
		{
			GameObject* gameObject = new GameObject();
			gameObject->SetLayerType(enums::eLayerType::Effect);
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			gameObject->SetCommonObject(true);
			Layer& myLayer = scene->GetLayer(enums::eLayerType::Effect);
			myLayer.AddGameObject(gameObject);

			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(Resources::Find<Material>(L"AnimationMaterial"));

			playerScript->AddEffectObject(gameObject);
			player->AddChildObject(gameObject);
		}

		// weapon 세팅
		GameObject* weaponObject = new GameObject();
		player->AddChildObject(weaponObject);
		weaponObject->SetLayerType(enums::eLayerType::Playable);
		weaponObject->SetCommonObject(true);
		myLayer.AddGameObject(weaponObject);
		MeshRenderer* weaponMR = weaponObject->AddComponent<MeshRenderer>();
		weaponMR->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		weaponMR->SetMaterial(Resources::Find<Material>(L"WeaponMaterial"));
		WeaponScript* weaponScript = playerScript->SetWeaponObject(weaponObject);

		// weapon effect 세팅
		for (int index = 0; index < WEAPON_EFFECT_POOL; index++)
		{
			GameObject* gameObject = new GameObject();
			gameObject->SetLayerType(enums::eLayerType::Effect);
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			gameObject->SetCommonObject(true);
			Layer& myLayer = scene->GetLayer(enums::eLayerType::Effect);
			myLayer.AddGameObject(gameObject);

			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(Resources::Find<Material>(L"AnimationMaterial"));

			weaponScript->AddEffectObject(gameObject);
			player->AddChildObject(gameObject);
		}

		// Weapon Projectile 세팅
		for (int index = 0; index < PLAYER_PROJECTILE_POOL; index++)
		{
			GameObject* gameObject = new GameObject();
			gameObject->SetLayerType(enums::eLayerType::PlayableAttackCollider);
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			gameObject->SetCommonObject(true);
			Layer& myLayer = scene->GetLayer(enums::eLayerType::PlayableAttackCollider);
			myLayer.AddGameObject(gameObject);

			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(Resources::Find<Material>(L"ProjectileMaterial"));

			weaponScript->AddProjectileObject(gameObject);
			player->AddChildObject(gameObject);
		}
		return player;
	}
	template <typename T>
	static T* InstantiateCreature(Scene* scene, const std::wstring& material)
	{
		T* obj = new T();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Creature);
		myLayer.AddGameObject(obj);

		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		obj->AddComponent<Rigidbody>();
		obj->AddComponent<Collider2D>();
		obj->AddComponent<Animator>();
		obj->Initialize();
		return obj;
	}
	static GameObject* InstantiateLandObject(Scene* scene, math::Vector3 location, math::Vector3 scale)
	{
		GameObject* obj = new GameObject();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Land);
		myLayer.AddGameObject(obj);

		Collider2D* collider = obj->AddComponent<Collider2D>();
		collider->SetName(L"LandCollider");
		collider->SetColliderDetection(Collider2D::eColliderDetection::Land);
		obj->Initialize();

		Transform* transform = obj->GetTransform();
		transform->SetPosition(location);
		transform->SetScale(scale);

		return obj;
	}
	static GameObject* InstantiateGridObject(Scene* scene, CameraObject* cameraObject)
	{
		GameObject* obj = new GameObject();
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Default);
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
		cameraObj->SetCommonObject(true);
		cameraObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -6.750f));
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Default);
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
		cameraObj->SetCommonObject(true);
		cameraObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -6.750f));
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Default);
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
		cameraObj->SetCommonObject(true);
		cameraObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -6.750f));
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Default);
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