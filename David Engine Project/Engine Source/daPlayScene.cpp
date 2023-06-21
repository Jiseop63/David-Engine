#include "daPlayScene.h"

#include "daResources.h"

#include "daGameObject.h"

#include "daTransform.h"
#include "daMesh.h"
#include "daMeshRenderer.h"

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
		GameObject* player = new GameObject();
		AddGameObject(eLayerType::Playable, player);
		MeshRenderer* playerRenderer = player->AddComponent<MeshRenderer>();
		playerRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		playerRenderer->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
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
