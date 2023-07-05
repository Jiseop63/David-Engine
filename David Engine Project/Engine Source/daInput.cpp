#include "daInput.h"
#include "daApplication.h"
#include "daRenderer.h"
#include "daGraphicDevice Dx11.h"

extern da::Application application;

namespace da
{
	int ASCII[(UINT)eKeyCode::END] =
	{
		//Alphabet
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		//Special Key
		VK_RETURN, VK_ESCAPE, VK_LSHIFT, VK_LMENU, VK_LCONTROL,
		VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_LBUTTON, VK_RBUTTON,

		//Num Pad
		VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,
		VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
		VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8,
		VK_NUMPAD9,

		//Num Line
		'0', '1', '2', '3', '4', '5',
		'6', '7', '8', '9',
	};

	std::vector<Input::Key> Input::mKeys;
	math::Vector2 Input::mMousePosition = Vector2::Zero;
	math::Vector3 Input::mMouseWorldPosition = Vector3::Zero;
	math::Vector3 Input::mMouseUIPosition = Vector3::Zero;

	void Input::Initialize()
	{
		for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
		{
			Key keyInfo;
			keyInfo.key = (eKeyCode)i;
			keyInfo.state = eKeyState::None;
			keyInfo.bPressed = false;

			mKeys.push_back(keyInfo);
		}
	}

	void Input::Update()
	{
		if (GetFocus())
		{
			for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
			{
				if (GetAsyncKeyState(ASCII[i]) & 0x8000)
				{
					// ���� �����ӿ��� ���� �־���
					if (mKeys[i].bPressed)
						mKeys[i].state = eKeyState::Pressed;
					else
						mKeys[i].state = eKeyState::Down;

					mKeys[i].bPressed = true;
				}
				else // ���� �����ӿ� Ű�� �������� �ʴ�.
				{
					// ���� �����ӿ� ��Ű�� �����־���.
					if (mKeys[i].bPressed)
						mKeys[i].state = eKeyState::Up;
					else
						mKeys[i].state = eKeyState::None;

					mKeys[i].bPressed = false;
				}
			}

			POINT mousePos = {};
			GetCursorPos(&mousePos);

			ScreenToClient(application.GetHwnd(), &mousePos);
			D3D11_VIEWPORT deviceViewport = graphics::GetDevice()->GetViewPort();
			Viewport viewport(deviceViewport.TopLeftX, deviceViewport.TopLeftY
			, deviceViewport.Width, deviceViewport.Height, deviceViewport.MinDepth, deviceViewport.MaxDepth);
			
			mMousePosition.x = (float)mousePos.x;
			mMousePosition.y = (float)mousePos.y;

			CalculateMouseMatrix();
		}
		else
		{
			for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
			{
				if (eKeyState::Down == mKeys[i].state
					|| eKeyState::Pressed == mKeys[i].state)
				{
					mKeys[i].state = eKeyState::Up;
				}
				else if (eKeyState::Up == mKeys[i].state)
				{
					mKeys[i].state = eKeyState::None;
				}

				mKeys[i].bPressed = false;
			}
		}
	}

	void Input::Render(HDC hdc)
	{
	}

	void Input::CalculateMouseMatrix()
	{

		if (renderer::mainCamera)
		{
			// �������
			Matrix worldMatrix = Matrix::Identity;
			// viewport ��������
			D3D11_VIEWPORT deviceViewport = graphics::GetDevice()->GetViewPort();
			Viewport viewport =
				Viewport(deviceViewport.TopLeftX, deviceViewport.TopLeftY
					, deviceViewport.Width, deviceViewport.Height, deviceViewport.MinDepth, deviceViewport.MaxDepth);
			// ����Ʈ���� ������ǥ���� ������� �����ؼ� ���콺 ��ġ ���ϱ�
			Matrix projMatrix = renderer::mainCamera->GetProjectionMatrix();
			Matrix viewMatrix = renderer::mainCamera->GetViewMatrix();
			Vector3 mousePosition = Vector3(mMousePosition.x, mMousePosition.y, 0.0f);

			viewport.Unproject(mousePosition, projMatrix, viewMatrix, worldMatrix, mMouseWorldPosition);
		}
		
		if (renderer::uiCamera)
		{
			// �������
			Matrix worldMatrix = Matrix::Identity;
			// viewport ��������
			D3D11_VIEWPORT deviceViewport = graphics::GetDevice()->GetViewPort();
			Viewport viewport =
				Viewport(deviceViewport.TopLeftX, deviceViewport.TopLeftY
					, deviceViewport.Width, deviceViewport.Height, deviceViewport.MinDepth, deviceViewport.MaxDepth);

			// ����Ʈ���� ������ǥ���� ������� �����ؼ� ���콺 ��ġ ���ϱ�
			Matrix projMatrix = renderer::uiCamera->GetProjectionMatrix();
			Matrix viewMatrix = renderer::uiCamera->GetViewMatrix();
			Vector3 mousePosition = Vector3(mMousePosition.x, mMousePosition.y, 0.0f);

			viewport.Unproject(mousePosition, projMatrix, viewMatrix, worldMatrix, mMouseUIPosition);
		}
	}
}
