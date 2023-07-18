#include "daCursorScript.h"

#include "daInput.h"

#include "daGameObject.h"
#include "daTransform.h"

namespace da
{
	CursorScript::CursorScript()
	{
	}
	CursorScript::~CursorScript()
	{
	}
	void CursorScript::LateUpdate()
	{
		// 내(cursor) Tr 가져오기
		Transform* cursorTransform = GetOwner()->GetComponent<Transform>();
		Vector3 myPosition = cursorTransform->GetPosition();

		// 마우스 위치 가져오기
		Vector3 uiMousePos = Input::GetMouseUIPosition();

		// 내위치를 마우스 위치로 옮기기
		cursorTransform->SetPosition(uiMousePos);
	}
}
