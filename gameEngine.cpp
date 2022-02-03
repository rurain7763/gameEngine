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

        if (key == CHANGE_LANG_BUTTON) inputMg->changeLanguageMode();

        if (key == VK_ESCAPE) inputMg->setKeyDown(KEY_ESCAPE);
        if (key == VK_PRIOR) inputMg->setKeyDown(KEY_PAGEUP);
        if (key == VK_NEXT) inputMg->setKeyDown(KEY_PAGEDOWN);
        if (key == VK_SPACE) inputMg->setKeyDown(KEY_space);

        return 0;
    }
    case WM_CHAR:
    {
        char key = (char)wParam;

        if (key == '1') inputMg->setKeyDown(KEY_1);
        if (key == '2') inputMg->setKeyDown(KEY_2);
        if (key == '3') inputMg->setKeyDown(KEY_3);
        if (key == '4') inputMg->setKeyDown(KEY_4);
        if (key == 'w') inputMg->setKeyDown(KEY_w);
        if (key == 's') inputMg->setKeyDown(KEY_s);
        if (key == 'a') inputMg->setKeyDown(KEY_a);
        if (key == 'd') inputMg->setKeyDown(KEY_d);
        if (key == 'p') inputMg->setKeyDown(KEY_p);
        if (key == 'q') inputMg->setKeyDown(KEY_q);
        if (key == 'e') inputMg->setKeyDown(KEY_e);
        if (key == 'r') inputMg->setKeyDown(KEY_r);
        if (key == 't') inputMg->setKeyDown(KEY_t);
        if (key == 'y') inputMg->setKeyDown(KEY_y);
        if (key == 'u') inputMg->setKeyDown(KEY_u);
        if (key == 'i') inputMg->setKeyDown(KEY_i);
        if (key == 'o') inputMg->setKeyDown(KEY_o);
        if (key == 'f') inputMg->setKeyDown(KEY_f);
        if (key == 'g') inputMg->setKeyDown(KEY_g);
        if (key == 'h') inputMg->setKeyDown(KEY_h);
        if (key == 'j') inputMg->setKeyDown(KEY_j);
        if (key == 'k') inputMg->setKeyDown(KEY_k);
        if (key == 'l') inputMg->setKeyDown(KEY_l);
        if (key == 'z') inputMg->setKeyDown(KEY_z);
        if (key == 'x') inputMg->setKeyDown(KEY_x);
        if (key == 'c') inputMg->setKeyDown(KEY_c);
        if (key == 'v') inputMg->setKeyDown(KEY_v);
        if (key == 'b') inputMg->setKeyDown(KEY_b);
        if (key == 'n') inputMg->setKeyDown(KEY_n);
        if (key == 'm') inputMg->setKeyDown(KEY_m);
        if (key == 'Q') inputMg->setKeyDown(KEY_Q);
        if (key == 'W') inputMg->setKeyDown(KEY_W);
        if (key == 'E') inputMg->setKeyDown(KEY_E);
        if (key == 'R') inputMg->setKeyDown(KEY_R);
        if (key == 'T') inputMg->setKeyDown(KEY_T);
        if (key == 'O') inputMg->setKeyDown(KEY_O);
        if (key == 'P') inputMg->setKeyDown(KEY_P);
    
        return 0;
    }
    case WM_KEYUP:
    {
        char key = (char)wParam;

        if (key == VK_ESCAPE) inputMg->setKeyUp(KEY_ESCAPE);
        if (key == VK_PRIOR) inputMg->setKeyUp(KEY_PAGEUP);
        if (key == VK_NEXT) inputMg->setKeyUp(KEY_PAGEDOWN);
        if (key == VK_SPACE) inputMg->setKeyUp(KEY_space);

        if (key == '1') inputMg->setKeyUp(KEY_1);
        if (key == '2') inputMg->setKeyUp(KEY_2);
        if (key == '3') inputMg->setKeyUp(KEY_3);
        if (key == '4') inputMg->setKeyUp(KEY_4);
        if (key == 'W')
        {
            inputMg->setKeyUp(KEY_w);
            inputMg->setKeyUp(KEY_W);
        }
        if (key == 'S') inputMg->setKeyUp(KEY_s);
        if (key == 'A') inputMg->setKeyUp(KEY_a);
        if (key == 'D') inputMg->setKeyUp(KEY_d);
        if (key == 'P')
        {
            inputMg->setKeyUp(KEY_p);
            inputMg->setKeyUp(KEY_P);
        }
        if (key == 'Q')
        {
            inputMg->setKeyUp(KEY_q);
            inputMg->setKeyUp(KEY_Q);
        }
        if (key == 'E')
        {
            inputMg->setKeyUp(KEY_e);
            inputMg->setKeyUp(KEY_E);
        }
        if (key == 'R')
        {
            inputMg->setKeyUp(KEY_r);
            inputMg->setKeyUp(KEY_R);
        }
        if (key == 'T')
        {
            inputMg->setKeyUp(KEY_t);
            inputMg->setKeyUp(KEY_T);
        }
        if (key == 'Y') inputMg->setKeyUp(KEY_y);
        if (key == 'U') inputMg->setKeyUp(KEY_u);
        if (key == 'I') inputMg->setKeyUp(KEY_i);
        if (key == 'O')
        {
            inputMg->setKeyUp(KEY_o);
            inputMg->setKeyUp(KEY_O);
        }
        if (key == 'F') inputMg->setKeyUp(KEY_f);
        if (key == 'G') inputMg->setKeyUp(KEY_g);
        if (key == 'H') inputMg->setKeyUp(KEY_h);
        if (key == 'J') inputMg->setKeyUp(KEY_j);
        if (key == 'K') inputMg->setKeyUp(KEY_k);
        if (key == 'L') inputMg->setKeyUp(KEY_l);
        if (key == 'Z') inputMg->setKeyUp(KEY_z);
        if (key == 'X') inputMg->setKeyUp(KEY_x);
        if (key == 'C') inputMg->setKeyUp(KEY_c);
        if (key == 'V') inputMg->setKeyUp(KEY_v);
        if (key == 'B') inputMg->setKeyUp(KEY_b);
        if (key == 'N') inputMg->setKeyUp(KEY_n);
        if (key == 'M') inputMg->setKeyUp(KEY_m);

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

