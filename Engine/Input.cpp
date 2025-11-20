#include "Input.h"


namespace Input
{
	using namespace DirectX;

	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	LPDIRECTINPUTDEVICE8 pMouseDevice = nullptr;
	DIMOUSESTATE mouseState; // マウスの状態保存用
	DIMOUSESTATE prevMouseState; // 前回のマウスの状態保存用

	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256] = { 0 };

	XMVECTOR mousePosition;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		// 非排他的、バックグラウンドモードに設定
		pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		// キーボードの状態を更新
		memcpy(prevKeyState, keyState, sizeof(keyState));
		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		// マウスの状態を更新
		memcpy(&prevMouseState, &mouseState, sizeof(DIMOUSESTATE));
		pMouseDevice->Acquire();
		pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		if (IsKey(keyCode) && !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if (!IsKey(keyCode) && (prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButton(int button)
	{
		if (mouseState.rgbButtons[button] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int button)
	{
		if (IsMouseButton(button) && !(prevMouseState.rgbButtons[button] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonUp(int button)
	{
		if (!IsMouseButton(button) && (prevMouseState.rgbButtons[button] & 0x80))
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
	}
	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 1.0f);		
	}
}