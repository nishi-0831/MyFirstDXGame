// MyFirstGame.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "framework.h"
#include "MyFirstDXGame.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Transform.h"
#include "Engine/Input.h"
#include "Engine/RootJob.h"
#include <string>
#include <wchar.h>
#include <tchar.h>
#include <combaseapi.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "Engine/Model.h"
#if 0
#include <d3d11.h>
//リンカ
#pragma comment(lib, "d3d11.lib")
#endif
#pragma comment(lib,"winmm.lib")

#define USE_IMGUI 0
#define MAX_LOADSTRING 100
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImGuiContext* ctx;

HWND hWnd = nullptr;
const TCHAR* WIN_CLASS_NAME = L"SAMPLE_GAME_WINDOW";
//SVGAサイズ
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

RootJob* pRootJob = nullptr;

#if USE_IMGUI
// アロケート関数
void* MyImGuiMalloc(size_t size, void* user_data)
{
    (void)user_data; // 未使用なら警告抑制
    return std::malloc(size);
}

// 解放関数
void MyImGuiFree(void* ptr, void* user_data)
{
    (void)user_data;
    std::free(ptr);
}
#endif

//const WCHAR* WIN_CLASS_NAME = L"SAMPLE_GAME_WINDOW";
//const char* WIN_CLASS_NAME = "SAMPLE_GAME_WINDOW";
//const std::string WIN_CLASS_NAME = "SAMPLE_GAME_WINDOW";

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
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

	pRootJob = new RootJob(nullptr);
	pRootJob->Initialize();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;

    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        //メッセージあり
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        timeBeginPeriod(1);
        static DWORD countFps = 0;
        static DWORD startTime = timeGetTime();
        DWORD nowTime = timeGetTime();
        static DWORD lastUpdateTime = nowTime;
        timeEndPeriod(1);
        // ミリ秒単位
        // 1000ミリ秒で1秒
        


        if (nowTime - startTime >= 1000)
        {
            std::string str = "FPS:" + std::to_string(nowTime - startTime)
                + ", " + std::to_string(countFps);

            SetWindowTextA(hWnd, str.c_str());

            countFps = 0;
            startTime = nowTime;
        }

        if (nowTime - lastUpdateTime <= 1000.0f / 60)
        {
            continue;
        }
        lastUpdateTime = nowTime;
        countFps++;

        //メッセージなし
        //else

        {
#if USE_IMGUI
            ImGui::SetCurrentContext(ctx);
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            /*ctx = ImGui::GetCurrentContext();
            ImGui::SetCurrentContext(ctx);*/
            if (show_demo_window)
            {
                //ImGui::ShowDemoWindow(&show_demo_window);
            }
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                
                if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                    {
                    ImGui::Text("This is some useful text.");
                    }
                else
                {
                    ImGui::Text("This is.");
                }

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }
#endif
            Camera::Update();
            Input::Update();
            pRootJob->UpdateSub();

            
            Direct3D::BeginDraw();
           
            //pRootJobから、全てのオブジェクトの描画を呼ぶ
			pRootJob->DrawSub();
#if USE_IMGUI
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
#endif
            Direct3D::EndDraw();
        }

    }
    

    
#if USE_IMGUI
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#endif

	pRootJob->ReleaseSub();
    Input::Release();
    Direct3D::Release();
    Model::Release();
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
    Input::Initialize(hWnd);

    if (FAILED(result))
    {
        _wassert(Direct3D::GetMsg(result), _CRT_WIDE(__FILE__), (unsigned int)__LINE__);
        //PostQuitMessage(0);
        return 0;
    }
    /*quad = new Quad();
    result =quad->Initialze();
    dice = new Dice();
    dice->Initialze();
    sprite = new Sprite();
    sprite->Initialze();*/

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

#if USE_IMGUI
   IMGUI_CHECKVERSION();

   ctx = ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
   io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
   
   ImGui_ImplWin32_EnableDpiAwareness();
   float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
   // Setup scaling
       ImGuiStyle & style = ImGui::GetStyle();
   style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
   style.FontScaleDpi = main_scale;        // Set initial font scale. (using 

   io.ConfigDpiScaleFonts = true;
   io.ConfigDpiScaleViewports = true;
   //if(io.ConfigFlags & ImGuiFon)

   // Setup Dear ImGui style
   ImGui::StyleColorsDark();
   //ImGui::StyleColorsLight();


   ImGui::SetCurrentContext(ctx);
   ImGui::SetAllocatorFunctions(MyImGuiMalloc, MyImGuiFree);

   ImGui_ImplWin32_Init(hWnd);
   ImGui_ImplDX11_Init(Direct3D::pDevice, Direct3D::pContext);

#endif
  
   ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


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
#if USE_IMGUI
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
#endif
    switch (message)
    {
    case WM_KEYDOWN:
        switch ((char)wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_MOUSEMOVE:
        // lParamの内、LOW WORD:左側?がx,HI WORD:右側?がyだよ
        Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		//OutputDebugStringA((std::to_string(LOWORD(lParam)) + "," + std::to_string(HIWORD(lParam)) + "\n").c_str());
        
        return 0;
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
