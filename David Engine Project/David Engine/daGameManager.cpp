#include "daGameManager.h"
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"
#include "daENVObject.h"
namespace da
{
	bool GameManager::mIsOverlayOn = false;
	void GameManager::DisappearObjects()
	{
		Layer* backgroundLayer = SceneManager::GetActiveScene()->GetLayer(enums::eLayerType::BackGround);
		Layer* UILayer = SceneManager::GetActiveScene()->GetLayer(enums::eLayerType::UI);

		std::vector<GameObject*> backgroundObjs = backgroundLayer->GetGameObjects();
		std::vector<GameObject*> UIObjs = UILayer->GetGameObjects();

		for (GameObject* targetObject : backgroundObjs)
		{
			dynamic_cast<ENVObject*>(targetObject)->DoDisappear();
		}
		for (GameObject* targetObject : UIObjs)
		{
			dynamic_cast<ENVObject*>(targetObject)->DoDisappear();

		}
	}
	void GameManager::TurnOnOverlay()
	{
		// 오버레이 객체를 찾기
		Layer* backgroundLayer = SceneManager::GetActiveScene()->GetLayer(enums::eLayerType::BackGround);
	}
}