#include "gameEngine.h"
#include "iStd.h"
#include "Game.h"

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
    float monitorX = monitorW / 2 - DEV_WIDTH / 2;
    float monitorY = monitorH / 2 - DEV_HEIGHT / 2;
    
    if (monitorX < 0.f || monitorY < 0.f)
    {
        monitorX = 0.f;
        monitorY = 0.f;
    }

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        monitorX, monitorY, DEV_WIDTH, DEV_HEIGHT,
        nullptr, nullptr, hInstance, nullptr);

    setWndRectInfo(monitorX, monitorY, DEV_WIDTH, DEV_HEIGHT);
    SetCursorPos(monitorX + monitorW / 2, monitorY + monitorH / 2);

    HDC hdc = GetDC(hWnd);

    if (!hWnd)
    {
        return 0;
    }

    loadGL(hWnd);
    SetMenu(hWnd, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEENGINE));

    MSG msg;

    loadGame();
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

    endGL();

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        RECT rt;
        GetClientRect(hWnd, &rt);

        float curW = rt.right - rt.left;
        float curH = rt.bottom - rt.top;
        setWndSizeInfo(curW, curH);        

        float ratio = DEV_WIDTH / DEV_HEIGHT;

        float h = curH;
        float w = h * ratio;
        int posX, posY;

        if (w < curW)
        {
            posX = (curW - w) / 2;
            posY = 0;

            setViewPort(posX, posY, w, h);
            return 0;
        }

        w = curW;
        h = w / ratio;

        if (h < curH)
        {
            posX = 0;
            posY = (curH - h) / 2;

            setViewPort(posX, posY, w, h);
            return 0;
        }

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

        if (key == 'W') inputMgt->setKeyDown(KEY_W);
        if (key == 'S') inputMgt->setKeyDown(KEY_S);
        if (key == 'A') inputMgt->setKeyDown(KEY_A);
        if (key == 'D') inputMgt->setKeyDown(KEY_D);
        if (key == 'P') inputMgt->setKeyDown(KEY_P);
        if (key == VK_ESCAPE) inputMgt->setKeyDown(KEY_ESCAPE);
        if (key == VK_PRIOR) inputMgt->setKeyDown(KEY_PAGEUP);
        if (key == VK_NEXT) inputMgt->setKeyDown(KEY_PAGEDOWN);

        return 0;
    }
    case WM_KEYUP:
    {
        char key = (char)wParam;

        if (key == 'W') inputMgt->setKeyUp(KEY_W);
        if (key == 'S') inputMgt->setKeyUp(KEY_S);
        if (key == 'A') inputMgt->setKeyUp(KEY_A);
        if (key == 'D') inputMgt->setKeyUp(KEY_D);
        if (key == 'P') inputMgt->setKeyUp(KEY_P);
        if (key == VK_ESCAPE) inputMgt->setKeyUp(KEY_ESCAPE);
        if (key == VK_PRIOR) inputMgt->setKeyUp(KEY_PAGEUP);
        if (key == VK_NEXT) inputMgt->setKeyUp(KEY_PAGEDOWN);

        return 0;
    }
    case WM_MOUSEMOVE:
    {
        float x = GET_X_LPARAM(lParam);
        float y = GET_Y_LPARAM(lParam);

        wrappingCursor(x, y);

        x = GET_X_LPARAM(lParam);
        y = GET_Y_LPARAM(lParam);

        iRect viewPort = getViewPort();
        iVector2f wRange = { viewPort.position.x, viewPort.position.x + viewPort.size.width };
        iVector2f hRange = { viewPort.position.y, viewPort.position.y + viewPort.size.height };

        x = ((x - wRange.x) / (wRange.y - wRange.x)) * DEV_WIDTH;
        y = ((y - hRange.x) / (hRange.y - hRange.x)) * DEV_HEIGHT;

        inputMgt->setMousePos(x, y);

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

