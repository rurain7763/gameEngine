#include "gameEngine.h"
#include "iStd.h"
#include "Game.h"
#include "TestScene.h"
#include "ServerScene.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[20];
WCHAR szWindowClass[20];

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, 10);
    LoadStringW(hInstance, IDC_GAMEENGINE, szWindowClass, 10);

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GAMEENGINE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    hInst = hInstance;

    float monitorW = GetSystemMetrics(SM_CXSCREEN);
    float monitorH = GetSystemMetrics(SM_CYSCREEN);
    float monitorX = monitorW / 2.f - monitorW / 4.f;
    float monitorY = monitorH / 2.f - monitorH / 4.f;
    
    if (monitorX < 0.f || monitorY < 0.f)
    {
        monitorX = 0.f;
        monitorY = 0.f;
    }

    float halfW = monitorW / 2.f;
    float halfH = monitorH / 2.f;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                              monitorX, monitorY, halfW, halfH,
                              nullptr, nullptr, hInstance, nullptr);

    setWndRectInfo(monitorX, monitorY, halfW, halfH);
    SetCursorPos(monitorX + halfW / 2, monitorY + halfH / 2);

    HDC hdc = GetDC(hWnd);

    if (!hWnd)
    {
        return 0;
    }

#if USE_SOCKET
    loadWSA();
#endif
    loadGdi();
    loadGL(hWnd);
    SetMenu(hWnd, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

#ifndef _DEBUG
    SetWindowPos(hWnd, HWND_TOPMOST,
                 0, 0, 0, 0,
                 SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
#endif

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEENGINE));

    MSG msg;

    loadGame();
    TestScene scene1;
    TestScene scene2;
    TestScene scene3;
    ServerScene scene4;
    
    sceneMg->addScene(&scene4);
    sceneMg->addScene(&scene1);
    sceneMg->addScene(&scene2);
    sceneMg->addScene(&scene3);
    
    while (isContinueApp())
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            clear();
            drawGame();
            swapBuffer(hdc);
        }
    }

    endGame();

#if USE_SOCKET
    endWSA();
#endif
    endGL();
    endGdi();

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SHOWWINDOW:
    {
        return 0;
    }
    case WM_SIZE:
    {
        RECT rt;
        GetClientRect(hWnd, &rt);

        float curW = rt.right - rt.left;
        float curH = rt.bottom - rt.top;
        setWndSizeInfo(curW, curH);        

        iRect viewPort = caculateViewPort({DEV_WIDTH, DEV_HEIGHT}, { curW, curH });
        setViewPort(viewPort.x, viewPort.y, viewPort.width, viewPort.height);

        return 0;
    }
    case WM_SIZING:
    {
        int which = LOWORD(wParam);

        if (which == WMSZ_BOTTOM)
        {

        }
        else if (which == WMSZ_BOTTOMLEFT)
        {

        }
        else if (which == WMSZ_BOTTOMRIGHT)
        {

        }
        else if (which == WMSZ_RIGHT)
        {

        }
        else if (which == WMSZ_TOP)
        {

        }
        else if (which == WMSZ_TOPLEFT)
        {

        }
        else if (which == WMSZ_TOPRIGHT)
        {

        }

        return 0;
    }
    case WM_KEYDOWN:
    {
        char key = (char)wParam;

        if (key == '1') inputMg->setKeyDown(KEY_1);
        if (key == '2') inputMg->setKeyDown(KEY_2);
        if (key == '3') inputMg->setKeyDown(KEY_3);
        if (key == '4') inputMg->setKeyDown(KEY_4);
        if (key == 'W') inputMg->setKeyDown(KEY_W);
        if (key == 'S') inputMg->setKeyDown(KEY_S);
        if (key == 'A') inputMg->setKeyDown(KEY_A);
        if (key == 'D') inputMg->setKeyDown(KEY_D);
        if (key == 'P') inputMg->setKeyDown(KEY_P);
        if (key == VK_ESCAPE) inputMg->setKeyDown(KEY_ESCAPE);
        if (key == VK_PRIOR) inputMg->setKeyDown(KEY_PAGEUP);
        if (key == VK_NEXT) inputMg->setKeyDown(KEY_PAGEDOWN);

        return 0;
    }
    case WM_KEYUP:
    {
        char key = (char)wParam;

        if (key == '1') inputMg->setKeyUp(KEY_1);
        if (key == '2') inputMg->setKeyUp(KEY_2);
        if (key == '3') inputMg->setKeyUp(KEY_3);
        if (key == '4') inputMg->setKeyUp(KEY_4);
        if (key == 'W') inputMg->setKeyUp(KEY_W);
        if (key == 'S') inputMg->setKeyUp(KEY_S);
        if (key == 'A') inputMg->setKeyUp(KEY_A);
        if (key == 'D') inputMg->setKeyUp(KEY_D);
        if (key == 'P') inputMg->setKeyUp(KEY_P);
        if (key == VK_ESCAPE) inputMg->setKeyUp(KEY_ESCAPE);
        if (key == VK_PRIOR) inputMg->setKeyUp(KEY_PAGEUP);
        if (key == VK_NEXT) inputMg->setKeyUp(KEY_PAGEDOWN);

        return 0;
    }
    case WM_MOUSEMOVE:
    {
        float x = GET_X_LPARAM(lParam);
        float y = GET_Y_LPARAM(lParam);

        wrappingCursor(x, y);
        coordMousePosToViewPort({ DEV_WIDTH, DEV_HEIGHT }, x, y);

        inputMg->setMousePos(x, y);

        return 0;
    }
    case WM_MOVE:
    {
        float x = LOWORD(lParam);
        float y = HIWORD(lParam);

        setWndPosInfo(x, y);
        return 0;
    }
    case WM_CLOSE:
    {
        shutDownApp();
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

