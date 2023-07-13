#include "guiEditor.h"
#include "..\\Engine Source\\daRenderer.h"
#include "..\\Engine Source\\daResources.h"
#include "..\\Engine Source\\daMesh.h"
#include "..\\Engine Source\\daMaterial.h"
#include "..\\Engine Source\\daMeshRenderer.h"
#include "..\\Engine Source\\daTransform.h"

#include "..\\Engine Source\\daMesh.h"

namespace gui
{
	using namespace da::enums;
	std::vector<Widget*> Editor::mWidgets = {};
	std::vector<EditorObject*> Editor::mEditorObjects = {};
	std::vector<DebugOjbect*> Editor::mDebugOjbects = {};
	void Editor::Initialize()
	{
	}
	void Editor::Run()
	{
	}
	void Editor::Update()
	{
	}
	void Editor::LateUpdate()
	{
	}
	void Editor::Render()
	{
	}
	void Editor::Release()
	{
	}
	void Editor::DebugRender(const da::graphics::DebugMesh& mesh)
	{
	}
}