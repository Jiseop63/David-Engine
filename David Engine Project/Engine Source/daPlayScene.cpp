#include "daPlayScene.h"
#include "daGameObject.h"
#include "daTransform.h"
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
		player->AddComponent<MeshRenderer>();
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
