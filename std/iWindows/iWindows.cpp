#include"iWindows.h"
#include"iStd.h"

static ULONG_PTR gdiplusToken;
static iRect wndRtInfo;

static bool wrapping = false;
static bool run = true;

void loadGdi()
{
    GdiplusStartupInput input;

    //use default gdiplus
    GdiplusStartup(&gdiplusToken, &input, NULL);
}

void endGdi()
{
    GdiplusShutdown(gdiplusToken);
}

void loadWSA()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed!\n");
    }
}

void endWSA()
{
    WSACleanup();
}

iRect getWndRectInfo()
{
    return wndRtInfo;
}

void setWndPosInfo(float x, float y)
{
    wndRtInfo.x = x;
    wndRtInfo.y = y;
}

void setWndSizeInfo(float width, float height)
{
    wndRtInfo.width = width;
    wndRtInfo.height = height;
}

void setWndRectInfo(iRect& rt)
{
    wndRtInfo = rt;
}

void setWndRectInfo(float x, float y, float width, float height)
{
    wndRtInfo.x = x;
    wndRtInfo.y = y;
    wndRtInfo.width = width;
    wndRtInfo.height = height;
}

void showCursor(bool show)
{
    if (show)
    {
        while (1)
        {
            int n = ShowCursor(true);
            if (n > -1) break;
        }
    }
    else
    {
        while (1)
        {
            int n = ShowCursor(false);
            if (n < 0) break;
        }
    }
}

void wrappingCursor(float& x, float& y)
{
    if (!wrapping) return;

    RECT clip;
    clip.left = wndRtInfo.x;
    clip.top = wndRtInfo.y;
    clip.right = wndRtInfo.x + wndRtInfo.width;
    clip.bottom = wndRtInfo.y + wndRtInfo.height;

    ClipCursor(&clip);

    float margin = min(wndRtInfo.width, wndRtInfo.height) * .04f;

    if (x < 0.f + margin)
        SetCursorPos(wndRtInfo.x + (wndRtInfo.width - margin),
                     wndRtInfo.y + y);
    else if (x > wndRtInfo.width - margin)
        SetCursorPos(wndRtInfo.x + margin,
                     wndRtInfo.y + y);

    if (y < 0.f + margin)
        SetCursorPos(wndRtInfo.x + x,
                     wndRtInfo.y + (wndRtInfo.height - margin));
    else if (y > wndRtInfo.height - margin)
        SetCursorPos(wndRtInfo.x + x,
                     wndRtInfo.y + margin);
}

void wrapCursor(bool wrap)
{
    wrapping = wrap;
    if (!wrap) ClipCursor(nullptr);
    else SetCursorPos(wndRtInfo.x + wndRtInfo.width / 2,
                      wndRtInfo.y + wndRtInfo.height / 2);
}

bool isContinueApp()
{
    return run;
}

void shutDownApp()
{
    run = false;
}

wchar_t* multiByteToWideChar(const char* str, ...)
{
    char buff[1024];
    formattedText(buff, str);

    int buffLen = strlen(buff);

    int len = MultiByteToWideChar(CP_UTF8, 0, buff, -1, NULL, 0);

    if (len <= 0) return NULL;
    
    wchar_t* r = new wchar_t[len + 1];
    MultiByteToWideChar(CP_UTF8, 0, buff, buffLen + 1, r, len);

    return r;
}

char* wideCharToMultiByte(const wchar_t* str)
{
    int wStrLen = lstrlenW(str);
    int len = WideCharToMultiByte(CP_UTF8, 0, str, wStrLen, NULL, 0, NULL, NULL);
    
    char* r = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, str, wStrLen, r, len, NULL, NULL);
    r[len] = 0;

    return r;
}





