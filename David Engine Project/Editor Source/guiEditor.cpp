#include "guiEditor.h"
#include "..\\Engine Source\\daRenderer.h"
#include "..\\Engine Source\\daResources.h"
#include "..\\Engine Source\\daMesh.h"
#include "..\\Engine Source\\daMaterial.h"
#include "..\\Engine Source\\daMeshRenderer.h"
#include "..\\Engine Source\\daTransform.h"

#include "daGridScript.h"


namespace gui
{
	using namespace da::enums;
	std::vector<Widget*> Editor::mWidgets = {};
	std::vector<EditorObject*> Editor::mEditorObjects = {};
	std::vector<DebugObject*> Editor::mDebugObjects = {};
	void Editor::Initialize()
	{
		// Debug Objects
		mDebugObjects.resize((UINT)eColliderShape::End);
		

		// Rect
		std::shared_ptr<da::Mesh> mesh
			= da::Resources::Find<da::Mesh>(L"DebugRect");
		std::shared_ptr<da::Material> material
			= da::Resources::Find<da::Material>(L"DebugMaterial");
		mDebugObjects[(UINT)eColliderShape::Rect] = new DebugObject();
		da::MeshRenderer* meshRenderer = 
			mDebugObjects[(UINT)eColliderShape::Rect]->AddComponent<da::MeshRenderer>();
		meshRenderer->SetMesh(mesh);
		meshRenderer->SetMaterial(material);

		// Circle
		mesh = da::Resources::Find<da::Mesh>(L"DebugCircle");
		mDebugObjects[(UINT)eColliderShape::Circle] = new DebugObject();
		meshRenderer =
			mDebugObjects[(UINT)eColliderShape::Circle]->AddComponent<da::MeshRenderer>();
		meshRenderer->SetMesh(mesh);
		meshRenderer->SetMaterial(material);


		//Editor Objects
		EditorObject* grid = new EditorObject();
		grid->SetName(L"Grid");
		da::Transform* gridTransform = grid->GetTransform();


		meshRenderer = grid->AddComponent<da::MeshRenderer>();
		meshRenderer->SetMesh(da::Resources::Find<da::Mesh>(L"RectMesh"));
		meshRenderer->SetMaterial(da::Resources::Find<da::Material>(L"GridMaterial"));
		da::GridScript* gridSc = grid->AddComponent<da::GridScript>();
		renderer::gridScript = gridSc;
		// 0번째 카메라는 mainCamera
		gridSc->SetCamera(renderer::mainCamera);

		mEditorObjects.push_back(grid);
	}
	void Editor::Run()
	{
		Update();
		LateUpdate();
		Render();
	}
	void Editor::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}
	void Editor::LateUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->LateUpdate();
		}
	}
	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}
		for (const da::graphics::DebugMesh& mesh : renderer::debugMeshs)
		{
			DebugRender(mesh);
		}
		renderer::debugMeshs.clear();
	}
	void Editor::Release()
	{
		for (auto widget : mWidgets)
		{
			delete widget;
			widget = nullptr;
		}

		for (auto editorObj : mEditorObjects)
		{
			delete editorObj;
			editorObj = nullptr;
		}

		for (auto debugObj : mDebugObjects)
		{
			delete debugObj;
			debugObj = nullptr;
		}
	}
	void Editor::DebugRender(const da::graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.Type];

		// Tr 정보 가져오기
		da::Transform* tr = debugObj->GetComponent<da::Transform>();

		da::math::Vector3 pos = mesh.Position;
		pos.z -= 0.01f;

		tr->SetPosition(pos);
		tr->SetScale(mesh.Scale);
		tr->SetRotation(mesh.Rotation);
		tr->LateUpdate();

		// 카메라 설정하기
		da::Camera* mainCamera = renderer::mainCamera;
		da::Camera::SetGPUViewMatrix(mainCamera->GetViewMatrix());
		da::Camera::SetGPUProjectionMatrix(mainCamera->GetProjectionMatrix());
		
		// 색상 변경
		debugObj->BindColor(mesh.Color);
		debugObj->Render();
	}
}