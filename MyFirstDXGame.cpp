// MyFirstGame.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "Direct3D.h"
#include "framework.h"
#include "MyFirstDXGame.h"
#include <string>
#include <wchar.h>
#include <tchar.h>
#include "Quad.h"
#include "Camera.h"
#include <combaseapi.h>
#include "Dice.h"



#if 0
#include <d3d11.h>
//リンカ
#pragma comment(lib, "d3d11.lib")
#endif
//#include <stdlib.h>

#define MAX_LOADSTRING 100
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}



HWND hWnd = nullptr;
const TCHAR* WIN_CLASS_NAME = L"SAMPLE_GAME_WINDOW";
//SVGAサイズ
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
#if 0
ID3D11Device* pDevice;
ID3D11DeviceContext* pContext;
IDXGISwapChain* pSwapChain;
ID3D11RenderTargetView* pRenderTargetView;
#endif
Quad* quad;
Dice* dice;
//const WCHAR* WIN_CLASS_NAME = L"SAMPLE_GAME_WINDOW";
//const char* WIN_CLASS_NAME = "SAMPLE_GAME_WINDOW";
//const std::string WIN_CLASS_NAME = "SAMPLE_GAME_WINDOW";

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CoInitialize(nullptr);
    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYFIRSTGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    
    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYFIRSTGAME));

    MSG msg;

    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        //メッセージあり
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //メッセージなし
        else

        {
            Camera::Update();

            Direct3D::BeginDraw();

            //Direct3D::Draw();
            
            //quad->Draw();
            dice->Draw();
            Direct3D::EndDraw();
           

        }

    }
    

    quad->Release();
    SAFE_DELETE(quad);

    Direct3D::Release();
    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYFIRSTGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    //ファイルとか何とかのメニューを表示するっぽい
    wcex.lpszMenuName   = NULL;
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYFIRSTGAME);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   //ウィンドウサイズの計算
   RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

   //クライアント領域のサイズからウィンドウサイズに変換してくれるっぽい
   //ウィンドウの大きさをwinRect(第一引数)に合わせてくれる(アジャスト)
   AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
   int winW = winRect.right - winRect.left;     //ウィンドウ幅
   int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

    hWnd = CreateWindowW
   (
       szWindowClass,
       WIN_CLASS_NAME,
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT,
       0,
       winW,
       winH,
       nullptr, nullptr, hInstance, nullptr);

    HRESULT result;

    result = Direct3D::Initialize(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);
    Camera::Initialize();
    if (FAILED(result))
    {
        _wassert(Direct3D::GetMsg(result), _CRT_WIDE(__FILE__), (unsigned int)__LINE__);
        //PostQuitMessage(0);
        return 0;
    }
    quad = new Quad();
    result =quad->Initialze();
    dice = new Dice();
    dice->Initialze();


    if (FAILED(result))
    {
        _wassert(Direct3D::GetMsg(result), _CRT_WIDE(__FILE__), (unsigned int)__LINE__);
        //PostQuitMessage(0);
        return 0;
    }

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch ((char)wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
