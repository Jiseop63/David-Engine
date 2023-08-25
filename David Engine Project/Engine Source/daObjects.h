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
		T* obj = InstantiateObject<T>(scene, enums::eLayerType::UI);
		GameObject* gameObject = dynamic_cast<GameObject*>(obj);
		gameObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, OverlayZ));
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));

		ButtonScript* uiScript = obj->AddComponent<ButtonScript>();
		uiScript->SetSlotTextures(Resources::Find<graphics::Texture>(first), Resources::Find<graphics::Texture>(second));

		return obj;
	}
	template <typename T>
	static T* InstantiateMultiTextureUI(Scene* scene, const std::wstring& material, const std::wstring& first, const std::wstring& second)
	{
		T* obj = InstantiateObject<T>(scene, enums::eLayerType::UI);
		GameObject* gameObject = dynamic_cast<GameObject*>(obj);
		gameObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, OverlayZ));
		MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(material));
		UIScript* uiScript = obj->AddComponent<UIScript>();
		uiScript->SetSlotTextures(Resources::Find<graphics::Texture>(first), Resources::Find<graphics::Texture>(second));
		return obj;
	}
	static GameObject* InstantiatePlayer(Scene* scene)
	{

		GameObject* player = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Playable, L"AnimationMaterial");
		player->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Playable);
		
		// 라이트
		{
			Light* playerLight = player->AddComponent<Light>();
			playerLight->SetRadius(2.5f);
			playerLight->SetLightType(enums::eLightType::Point);
			playerLight->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));
		}
		// 플레이어 스크립트 추가
		PlayerScript* playerScript = player->AddComponent<PlayerScript>();
		// 플레이어 이펙트 세팅
		for (int index = 0; index < PLAYER_EFFECT_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
			gameObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			player->AddChildObject(gameObject);
			playerScript->AddEffectObject(gameObject);
		}

		// weapon 세팅
		GameObject* weaponObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Playable, L"WeaponMaterial");
		player->AddChildObject(weaponObject);
		WeaponScript* weaponScript = playerScript->SetWeaponObject(weaponObject);

		// weapon effect 세팅
		for (int index = 0; index < WEAPON_EFFECT_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
			gameObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			weaponScript->AddEffectObject(gameObject);
			player->AddChildObject(gameObject);
		}

		// Weapon Projectile 세팅
		for (int index = 0; index < PLAYER_PROJECTILE_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::PlayableAttackCollider, L"ProjectileMaterial");
			gameObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			weaponScript->AddProjectileObject(gameObject);
			player->AddChildObject(gameObject);
		}
		return player;
	}

	template <typename T>
	static T* InstantiateCreature(Scene* scene)
	{
		// enemyObject 추가
		GameObject* enemyObject = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Creature, L"AnimationMaterial");
		enemyObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
		// CreatureScript 추가
		T* enemyScript = enemyObject->AddComponent<T>();
		CreatureScript* creatureScript = dynamic_cast<CreatureScript*>(enemyScript);
		// Effect 추가
		GameObject* effectObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
		effectObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
		effectObject->SetObjectState(GameObject::eObjectState::Inactive);
		enemyObject->AddChildObject(effectObject);
		creatureScript->AddEffectObject(effectObject);

		// weapon 추가
		GameObject* enemyWeaponObj = InstantiateGameObject<GameObject>(scene, enums::eLayerType::CreatureAttackCollider, L"AnimationMaterial");
		enemyWeaponObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
		creatureScript->SetEnemyWeaponScript(enemyWeaponObj);

		// life Bar 추가해야함
		GameObject* enemyLifeObj = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Creature, L"CreatureLifeBarMaterial");
		enemyLifeObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
		CreatureLifebarScript* creatureLifeScript = creatureScript->SetCreatureLifeScript(enemyLifeObj);
		enemyLifeObj->GetTransform()->SetScale(1.0f, 0.250f, 1.0f);


		return enemyScript;
	}
	template <typename T>
	static T* InstantiateBoss(Scene* scene)
	{
		// enemyObject 추가
		GameObject* enemyObject = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Boss, L"AnimationMaterial");
		enemyObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CreatureZ));
		// BossScript 추가
		T* enemyScript = enemyObject->AddComponent<T>();

		return enemyScript;
	}

	// 이건 안씀
	static GameObject* InstantiateSkel(Scene* scene)
	{
		GameObject* enemyObject = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Creature, L"AnimationMaterial");
	}

	static GameObject* InstantiateLandObject(Scene* scene, math::Vector3 location, math::Vector3 scale)
	{
		GameObject* obj = new GameObject();
		obj->SetLayerType(enums::eLayerType::Land);
		obj->SetName(L"LandCollider");
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Land);
		myLayer.AddGameObject(obj);

		Collider2D* collider = obj->AddComponent<Collider2D>();
		collider->SetDetectionType(Collider2D::eDetectionType::Env);
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