#include "daPlayScene.h"

#include "daResources.h"

#include "daGameObject.h"

#include "daTransform.h"
#include "daMesh.h"
#include "daMeshRenderer.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

#include "daBackGroundObject.h"
#include "daBackGroundScript.h"

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
		srand((unsigned int)time(0));
		AddBackgroundObject();

		{
			CameraObject* cameraObj = new CameraObject();
			cameraObj->Initialize();
			AddGameObject(eLayerType::None, cameraObj);
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

	void PlayScene::AddBackgroundObject()
	{
		
	}
}
