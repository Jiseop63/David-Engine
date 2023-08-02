#pragma once
#include "daScene.h"

#include "..\\David Engine\\daCameraScript.h"
#include "..\\David Engine\\daSubCameraScript.h"
#include "..\\David Engine\\daPlayerScript.h"
#include "..\\David Engine\\daInventoryScript.h"
#include "..\\David Engine\\daLifeBarScript.h"
#include "..\\David Engine\\daDashCountScript.h"


namespace da
{
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Destroy();
		static void Release();
	public:
		template <typename T>
		static bool CreateScene(std::wstring name)
		{
			std::map<std::wstring, Scene*>::iterator iter
				= mScenes.find(name);

			if (iter != mScenes.end())
				return false;

			T* scene = new T();
			mScenes.insert(std::make_pair(name, scene));
			scene->Initialize();

			return true;
		}
		static Scene* GetActiveScene() { return mActiveScene; }
		static Scene* LoadScene(std::wstring name);

		static void MoveCommonObjects();


	public:
		static void SetMainCameraObject(GameObject* mainCamera) { mMainCameraScript = mainCamera->GetComponent<CameraScript>(); }
		static CameraScript* GetMainCameraScript() { return mMainCameraScript; }
		static void SetSubCameraObject(GameObject* subCamera) { mSubCameraScript = subCamera->GetComponent<SubCameraScript>(); }
		static SubCameraScript* GetSubCameraScript() { return mSubCameraScript; }

		static void SetPlayerObject(GameObject* player) { mPlayerScript = player->GetComponent<PlayerScript>(); }
		static PlayerScript* GetPlayerScript() { return mPlayerScript; }
		static void SetInventoryObject(GameObject* inventory) { mInventoryScript = inventory->GetComponent<InventoryScript>(); }
		static InventoryScript* GetInventoryScript() { return mInventoryScript; }

		static void SetLifebarObject(GameObject* lifebar) { mLifebarScript = lifebar->GetComponent<LifeBarScript>(); }
		static LifeBarScript* GetLifebarScript() { return mLifebarScript; }
		static void SetDashCountObject(GameObject* dashCount) { mDashCountScript = dashCount->GetComponent<DashCountScript>(); }
		static DashCountScript* GetDashCountScript() { return mDashCountScript; }


	private:
		static Scene* mActiveScene;
		static std::map<std::wstring, Scene*> mScenes;

	private:
		// camera
		static CameraScript*	mMainCameraScript;
		static SubCameraScript* mSubCameraScript;
		// player & inventory
		static PlayerScript*	mPlayerScript;
		static InventoryScript* mInventoryScript;
		// hud
		static LifeBarScript* mLifebarScript;
		static DashCountScript* mDashCountScript;
	};
}