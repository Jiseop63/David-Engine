#pragma once
#include "..\\Engine SOURCE\\CommonInclude.h"
#include "..\\Engine SOURCE\\David Engine.h"
#include "..\\Engine SOURCE\\daGraphics.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
	class Editor
	{
	public:
		static void Initialize();
		static void Run();

		static void Update();
		static void LateUpdate();
		static void Render();
		static void Release();

		static void InitializeDearImgui();
		static void RenderDearImgui();

	private:
		static void InitializeObjects();
		static void DebugRender(const da::graphics::DebugMesh& mesh);
	private:
		static std::vector<class Widget*> mWidgets;
		static std::vector<class EditorObject*> mEditorObjects;
		static std::vector<class DebugObject*> mDebugObjects;
	};
}
