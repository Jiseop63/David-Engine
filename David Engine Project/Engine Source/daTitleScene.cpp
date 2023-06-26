#include "daTitleScene.h"

#include "daResources.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daTransform.h"

#include "daCameraObject.h"
namespace da
{
	TitleScene::TitleScene()
	{
	}
	TitleScene::~TitleScene()
	{
	}
	void TitleScene::Initialize()
	{
		//BG
		{
			GameObject* backGround = new GameObject();
			AddGameObject(enums::eLayerType::BackGround, backGround);
			MeshRenderer* backGroundRenderer = backGround->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"TitleBackGroundMaterial"));
			Transform* backGroundTr = backGround->GetComponent<Transform>();
			backGroundTr->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
		}
		// BGObj
		{
			GameObject* cloudObj = new GameObject();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"CloudAMaterial"));
			Transform* backGroundTr = cloudObj->GetComponent<Transform>();
			backGroundTr->SetPosition(math::Vector3(-3.0f, -2.0f, 0.0f));
		}
		// BGObj
		{
			GameObject* cloudObj = new GameObject();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"CloudBMaterial"));
			Transform* backGroundTr = cloudObj->GetComponent<Transform>();
			backGroundTr->SetPosition(math::Vector3(3.0f, -2.0f, 0.0f));
		}
		// BGObj
		{
			GameObject* cloudObj = new GameObject();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"CloudCMaterial"));
			Transform* backGroundTr = cloudObj->GetComponent<Transform>();
			backGroundTr->SetPosition(math::Vector3(5.0f, 4.0f, 0.0f));
		}
		// BGObj
		{
			GameObject* titleLogoObj = new GameObject();
			AddGameObject(enums::eLayerType::BackGround, titleLogoObj);
			MeshRenderer* backGroundRenderer = titleLogoObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"TitleLogoMaterial"));
			Transform* backGroundTr = titleLogoObj->GetComponent<Transform>();
			backGroundTr->SetPosition(math::Vector3(0.0f, 5.0f, 0.0f));
		}
		{
			CameraObject* cameraObj = new CameraObject();
			cameraObj->Initialize();
			AddGameObject(enums::eLayerType::None, cameraObj);
		}
	}
	void TitleScene::Update()
	{
	}
	void TitleScene::LateUpdate()
	{
	}
	void TitleScene::Render()
	{
	}
}
