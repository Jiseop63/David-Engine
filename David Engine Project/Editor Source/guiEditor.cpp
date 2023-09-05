#include "guiEditor.h"
#include "..\\Engine Source\\daRenderer.h"
#include "..\\Engine Source\\daResources.h"
#include "..\\Engine Source\\daMesh.h"
#include "..\\Engine Source\\daMaterial.h"
#include "..\\Engine Source\\daMeshRenderer.h"
#include "..\\Engine Source\\daTransform.h"

#include "guiWidget.h"
#include "guiEditorObject.h"
#include "guiDebugObject.h"

#include "daGridScript.h"
#include "David Engine/daGameDataManager.h"
#include "Engine Source/daApplication.h"
#include "Engine Source/daGraphicDevice Dx11.h"
extern da::Application application;
extern da::GameDataManager gameManager;
namespace gui
{
	using namespace da::enums;
	std::vector<Widget*> Editor::mWidgets = {};
	std::vector<EditorObject*> Editor::mEditorObjects = {};
	std::vector<DebugObject*> Editor::mDebugObjects = {};

	da::GameDataManager gameManager;
	void Editor::Initialize()
	{
		InitializeDearImgui();
		InitializeObjects();
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
		RenderDearImgui();
		if (gameManager.IsDebuging())
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

		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
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
	void Editor::InitializeDearImgui()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(application.GetHwnd());
		ImGui_ImplDX11_Init(da::graphics::GetDevice()->GetID3D11Device(), da::graphics::GetDevice()->GetID3D11DeviceContext());

		// Load Fonts

	}
	void Editor::InitializeObjects()
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
	void Editor::RenderDearImgui()
	{
		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		//da::math::Vector4 clear_color(0.45f, 0.55f, 0.60f, 1.00f);
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows		
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}