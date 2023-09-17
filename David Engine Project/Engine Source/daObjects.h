#pragma
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"

#include "daResources.h"
#include "daMesh.h"
#include "daMaterial.h"

#include "daObjectsFastIncludeHeader.h"

#define PLAYER_EFFECT_POOL 20
#define PLAYER_PROJECTILE_POOL 15
#define PLAYER_WEAPON_EFFECT_POOL 15

#define MONSTER_PROJECTILE_POOL 15
#define MONSTER_WEAPON_EFFECT_POOL 15

namespace da::objects
{
#pragma region Basic GameObjects Func
	
	// ��ġ���� ����ִ� Parent ������ ���
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
	// �⺻������ ���
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
	
	// UI�� ����
	template <typename T>
	static T* InstantiateButtonObject(Scene* scene, const std::wstring& material, const std::wstring& first, const std::wstring& second)
	{
		T* obj = InstantiateObject<T>(scene, enums::eLayerType::UI);
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
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Playable);
		
		// ����Ʈ
		{
			Light* playerLight = player->AddComponent<Light>();
			playerLight->SetRadius(3.0f);
			playerLight->SetLightType(enums::eLightType::Point);
			playerLight->SetColor(math::Vector4(0.450f, 0.450f, 0.450f, 1.0f));
		}
		// �÷��̾� ��ũ��Ʈ �߰�
		PlayerScript* playerScript = player->AddComponent<PlayerScript>();
		SceneManager::AddPlayerObject(player);

		// �÷��̾� ����Ʈ ����
		for (int index = 0; index < PLAYER_EFFECT_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
			player->AddChildObject(gameObject);
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			playerScript->AddEffectObject(gameObject);
		}

		// weapon ����
		GameObject* weaponObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Playable, L"WeaponMaterial");
		player->AddChildObject(weaponObject);
		PlayerCombatScript* weaponScript = playerScript->SetWeaponObject(weaponObject);

		// weapon effect ����
		for (int index = 0; index < PLAYER_WEAPON_EFFECT_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			weaponScript->AddEffectObject(gameObject);
			player->AddChildObject(gameObject);
		}

		// Weapon Projectile ����
		for (int index = 0; index < PLAYER_PROJECTILE_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::PlayableProjectile, L"ProjectileMaterial");
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			weaponScript->AddProjectileObject(gameObject);
			player->AddChildObject(gameObject);
		}
		return player;
	}

	static SkelScript* InstantiateSkel(Scene* scene)
	{
		// enemyObject �߰�
		GameObject* enemyObject = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Monster, L"AnimationMaterial");
		enemyObject->SetName(L"SkelObject");
		// MonsterScript �߰�
		SkelScript* enemyScript = enemyObject->AddComponent<SkelScript>();
		MonsterScript* monsterScript = dynamic_cast<MonsterScript*>(enemyScript);
		enemyScript->SetName(L"SkelScript");
		// Effect �߰�
		GameObject* effectObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
		enemyObject->AddChildObject(effectObject);
		effectObject->SetObjectState(GameObject::eObjectState::Inactive);
		monsterScript->AddEffectObject(effectObject);

		// weapon ����
		GameObject* enemyWeaponObj = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Monster, L"AnimationMaterial");
		enemyWeaponObj->SetName(L"SkelWeaponScript");
		enemyObject->AddChildObject(enemyWeaponObj);
		SkelCombatScript* enemyWeaponScript = enemyWeaponObj->AddComponent<SkelCombatScript>();
		monsterScript->SetEnemyWeaponScript(enemyWeaponScript);
		enemyWeaponScript->AddOwnerScript(monsterScript);
		for (int index = 0; index < MONSTER_PROJECTILE_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::MonsterProjectile, L"ProjectileMaterial");
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			enemyWeaponScript->AddProjectileObject(gameObject);
			enemyObject->AddChildObject(gameObject);
		}

		// life Bar �߰��ؾ���
		GameObject* enemyLifeObj = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Monster, L"CreatureLifeBarMaterial");
		enemyLifeObj->SetName(L"SkelLifeBarObject");
		CreatureLifebarScript* creatureLifeBarScript = enemyLifeObj->AddComponent<CreatureLifebarScript>();
		creatureLifeBarScript->SetName(L"SkelLifeBarScript");
		monsterScript->SetCreatureLifeScript(creatureLifeBarScript);
		creatureLifeBarScript->SetValue(&monsterScript->GetCreatureStat()->MaxHP, &monsterScript->GetCreatureStat()->CurHP);
		enemyLifeObj->GetTransform()->SetScale(1.0f, 0.250f, 1.0f);

		return enemyScript;
	}
	static BansheeScript* InstantiateBanshee(Scene* scene)
	{
		// enemyObject �߰�
		GameObject* enemyObject = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Monster, L"AnimationMaterial");
		enemyObject->SetName(L"BansheeObject");
		// MonsterScript �߰�
		BansheeScript* enemyScript = enemyObject->AddComponent<BansheeScript>();
		enemyScript->SetName(L"BansheeScript");
		MonsterScript* monsterScript = dynamic_cast<MonsterScript*>(enemyScript);

		// Effect �߰�
		GameObject* effectObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
		enemyObject->AddChildObject(effectObject);
		effectObject->SetObjectState(GameObject::eObjectState::Inactive);
		monsterScript->AddEffectObject(effectObject);

		// weapon ����
		GameObject* enemyWeaponObj = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Monster, L"AnimationMaterial");
		enemyWeaponObj->SetName(L"BansheeWeaponScript");
		enemyObject->AddChildObject(enemyWeaponObj);
		BansheeCombatScript* enemyWeaponScript = enemyWeaponObj->AddComponent<BansheeCombatScript>();
		monsterScript->SetEnemyWeaponScript(enemyWeaponScript);
		enemyWeaponScript->AddOwnerScript(monsterScript);
		for (int index = 0; index < MONSTER_WEAPON_EFFECT_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::Effect, L"AnimationMaterial");
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			enemyWeaponScript->AddEffectObject(gameObject);
			enemyObject->AddChildObject(gameObject);
		}
		for (int index = 0; index < MONSTER_PROJECTILE_POOL; index++)
		{
			GameObject* gameObject = InstantiateCommonObject<GameObject>(scene, enums::eLayerType::MonsterProjectile, L"ProjectileMaterial");
			gameObject->SetObjectState(GameObject::eObjectState::Inactive);
			enemyWeaponScript->AddProjectileObject(gameObject);
			enemyObject->AddChildObject(gameObject);
		}


		// life Bar �߰��ؾ���
		GameObject* enemyLifeObj = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Monster, L"CreatureLifeBarMaterial");
		enemyLifeObj->SetName(L"BansheeLifeBarObject");
		CreatureLifebarScript* creatureLifeBarScript = enemyLifeObj->AddComponent<CreatureLifebarScript>();
		creatureLifeBarScript->SetName(L"BansheeLifeBarScript");
		monsterScript->SetCreatureLifeScript(creatureLifeBarScript);
		creatureLifeBarScript->SetValue(&monsterScript->GetCreatureStat()->MaxHP, &monsterScript->GetCreatureStat()->CurHP);
		enemyLifeObj->GetTransform()->SetScale(1.0f, 0.250f, 1.0f);

		return enemyScript;
	}
	template <typename T>
	static T* InstantiateBoss(Scene* scene)
	{
		// enemyObject �߰�
		GameObject* enemyObject = InstantiateGameObject<GameObject>(scene, enums::eLayerType::Boss, L"AnimationMaterial");
		// BossScript �߰�
		T* enemyScript = enemyObject->AddComponent<T>();

		return enemyScript;
	}

	static GameObject* InstantiateLandObject(Scene* scene, math::Vector3 location, math::Vector3 scale)
	{
		GameObject* obj = new GameObject();
		obj->SetLayerType(enums::eLayerType::Land);
		obj->SetName(L"LandObject");
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Land);
		myLayer.AddGameObject(obj);

		Collider2D* collider = obj->AddComponent<Collider2D>();
		collider->SetName(L"LandCollider");
		collider->SetDetectionType(Collider2D::eDetectionType::Env);
		obj->Initialize();

		Transform* transform = obj->GetTransform();
		transform->SetPosition(location);
		transform->SetScale(scale);

		return obj;
	}

	static GameObject* InstantiatePlatformObject(Scene* scene, math::Vector3 location, math::Vector3 scale)
	{
		GameObject* obj = new GameObject();
		obj->SetLayerType(enums::eLayerType::Platform);
		obj->SetName(L"PlatformObject");
		Layer& myLayer = scene->GetLayer(enums::eLayerType::Platform);
		myLayer.AddGameObject(obj);

		Collider2D* collider = obj->AddComponent<Collider2D>();
		collider->SetName(L"PlatformCollider");
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
		cameraObj->AddComponent<AudioListener>();

		// ���� �߿���
		cameraObj->Initialize();
		camera->TurnLayerMask(enums::eLayerType::Background, false);
		camera->TurnLayerMask(enums::eLayerType::Layer, false);
		camera->TurnLayerMask(enums::eLayerType::Light, false);
		camera->TurnLayerMask(enums::eLayerType::UI, false);
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

		// ���� �߿���
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

		// ���� �߿���
		camera->DisableLayerMask();
		camera->TurnLayerMask(enums::eLayerType::Background);
		camera->TurnLayerMask(enums::eLayerType::Layer);
		return cameraObj;
	}
#pragma endregion

}