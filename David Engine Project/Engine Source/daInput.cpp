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
					// 이전 프레임에도 눌려 있었다
					if (mKeys[i].bPressed)
						mKeys[i].state = eKeyState::Pressed;
					else
						mKeys[i].state = eKeyState::Down;

					mKeys[i].bPressed = true;
				}
				else // 현재 프레임에 키가 눌려있지 않다.
				{
					// 이전 프레임에 내키가 눌려있엇다.
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
			// 단위행렬
			Matrix worldMatrix = Matrix::Identity;
			// viewport 가져오기
			D3D11_VIEWPORT deviceViewport = graphics::GetDevice()->GetViewPort();
			Viewport viewport =
				Viewport(deviceViewport.TopLeftX, deviceViewport.TopLeftY
					, deviceViewport.Width, deviceViewport.Height, deviceViewport.MinDepth, deviceViewport.MaxDepth);
			// 뷰포트부터 월드좌표까지 역행렬을 적용해서 마우스 위치 구하기
			Matrix projMatrix = renderer::mainCamera->GetProjectionMatrix();
			Matrix viewMatrix = renderer::mainCamera->GetViewMatrix();
			Vector3 mousePosition = Vector3(mMousePosition.x, mMousePosition.y, 0.0f);

			viewport.Unproject(mousePosition, projMatrix, viewMatrix, worldMatrix, mMouseWorldPosition);
		}
		
		if (renderer::uiCamera)
		{
			// 단위행렬
			Matrix worldMatrix = Matrix::Identity;
			// viewport 가져오기
			D3D11_VIEWPORT deviceViewport = graphics::GetDevice()->GetViewPort();
			Viewport viewport =
				Viewport(deviceViewport.TopLeftX, deviceViewport.TopLeftY
					, deviceViewport.Width, deviceViewport.Height, deviceViewport.MinDepth, deviceViewport.MaxDepth);

			// 뷰포트부터 월드좌표까지 역행렬을 적용해서 마우스 위치 구하기
			Matrix projMatrix = renderer::uiCamera->GetProjectionMatrix();
			Matrix viewMatrix = renderer::uiCamera->GetViewMatrix();
			Vector3 mousePosition = Vector3(mMousePosition.x, mMousePosition.y, 0.0f);

			viewport.Unproject(mousePosition, projMatrix, viewMatrix, worldMatrix, mMouseUIPosition);
		}
	}
}
