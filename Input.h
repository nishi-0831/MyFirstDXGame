#pragma once

#include <dInput.h>
#include <DirectXMath.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	enum MouseCode
	{
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2,
	};
	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);
	bool IsMouseButton(int button);
	bool IsMouseButtonDown(int button);
	bool IsMouseButtonUp(int button);
	void Release();
	DirectX::XMVECTOR GetMousePosition();
	void SetMousePosition(int x, int y);

};
