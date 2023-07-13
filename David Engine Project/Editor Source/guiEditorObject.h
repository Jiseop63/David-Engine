#pragma once
#include "..\\Engine SOURCE\\daGameObject.h"

namespace gui
{
	class EditorObject : public da::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}