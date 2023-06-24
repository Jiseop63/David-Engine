#include "daPlayScene.h"

#include "daResources.h"

#include "daGameObject.h"

#include "daTransform.h"
#include "daMesh.h"
#include "daMeshRenderer.h"

#include "daCamera.h"
#include "daCameraScript.h"

namespace da
{
	using namespace enums;

	PlayScene::PlayScene()
	{
	}
	PlayScene::~PlayScene()
	{
	}
	void PlayScene::Initialize()
	{
		{
			GameObject* player = new GameObject();
			AddGameObject(eLayerType::Playable, player);
			MeshRenderer* playerRenderer = player->AddComponent<MeshRenderer>();
			playerRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			playerRenderer->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
			Transform* playerTr = player->GetComponent<Transform>();
			playerTr->SetPosition(math::Vector3(0.5f, 0.5f, 0.0f));
		}

		{
			GameObject* cameraObj = new GameObject();
			AddGameObject(eLayerType::None, cameraObj);
			cameraObj->GetComponent<Transform>()->SetPosition(math::Vector3(0.0f, 0.0f, -10.0f));
			Camera* cameraComp = cameraObj->AddComponent<Camera>();
			CameraScript* cameraScript = cameraObj->AddComponent<CameraScript>();
		}
	}
	void PlayScene::Update()
	{
		Scene::Update();
	}
	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void PlayScene::Render()
	{
		Scene::Render();
	}
}
