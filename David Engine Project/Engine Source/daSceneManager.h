#pragma once
#include "daScene.h"

#include "..\\David Engine\\daCameraScript.h"
#include "..\\David Engine\\daSubCameraScript.h"
#include "..\\David Engine\\daPlayerScript.h"
#include "..\\David Engine\\daInventoryScript.h"
#include "..\\David Engine\\daLifeBarScript.h"
#include "..\\David Engine\\daDashCountScript.h"
#include "..\\David Engine\\daCursorScript.h"

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

	public:
		static void AddLightObject(GameObject* light) { mLightObject = light; }
		static GameObject* GetLightObject() { return mLightObject; }

		static void AddMainCameraObject(GameObject* mainCamera) { mMainCameraScript = mainCamera->GetComponent<CameraScript>(); }
		static CameraScript* GetMainCameraScript() { return mMainCameraScript; }
		static void AddSubCameraObject(GameObject* subCamera) { mSubCameraScript = subCamera->GetComponent<SubCameraScript>(); }
		static SubCameraScript* GetSubCameraScript() { return mSubCameraScript; }

		static void AddPlayerObject(GameObject* player) { mPlayerScript = player->GetComponent<PlayerScript>(); }
		static PlayerScript* GetPlayerScript() { return mPlayerScript; }
		static void SetInventoryObject(GameObject* inventory) { mInventoryObject = inventory; }
		static GameObject* GetInventoryObject() { return mInventoryObject; }
		static void SetInventoryScript(GameObject* inventory) { mInventoryScript = inventory->GetComponent<InventoryScript>(); }
		static InventoryScript* GetInventoryScript() { return mInventoryScript; }

		static void AddHUDObject(GameObject* hud) { mHUDObject = hud; }
		static GameObject* GetHUDObject() { return mHUDObject; }
		static void AddLifebarObject(GameObject* lifebar) { mLifebarScript = lifebar->GetComponent<LifeBarScript>(); }
		static LifeBarScript* GetLifebarScript() { return mLifebarScript; }
		static void AddDashCountObject(GameObject* dashCount) 
		{ 
			mDashCountScript = dashCount->GetComponent<DashCountScript>(); 
			int a = 0;
		}
		static DashCountScript* GetDashCountScript() { return mDashCountScript; }
		
		static void AddCursourObject(GameObject* cursour) { mCursourScrip = cursour->GetComponent<CursorScript>(); }
		static CursorScript* GetCursourScrip() { return mCursourScrip; }

	private:
		static Scene* mActiveScene;
		static std::map<std::wstring, Scene*> mScenes;

	private:
		// light
		static GameObject*		mLightObject;
		// camera
		static CameraScript*	mMainCameraScript;
		static SubCameraScript* mSubCameraScript;
		// player & inventory
		static PlayerScript*	mPlayerScript;
		static GameObject*		mInventoryObject;
		static InventoryScript* mInventoryScript;
		// hud
		static GameObject*		mHUDObject;
		static LifeBarScript*	mLifebarScript;
		static DashCountScript* mDashCountScript;

		//cursour
		static CursorScript*	mCursourScrip;
	};
}