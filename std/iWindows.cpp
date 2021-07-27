#include"iWindows.h"
#include"iStd.h"

static iRect wndRtInfo;
static bool wrapping = false;
static bool run = true;

iRect getWndRectInfo()
{
    return wndRtInfo;
}

void setWndPosInfo(float x, float y)
{
    wndRtInfo.position = { x,y };
}

void setWndSizeInfo(float width, float height)
{
    wndRtInfo.size = { width, height };
}

void setWndRectInfo(iRect& rt)
{
    wndRtInfo.position = rt.position;
    wndRtInfo.size = rt.size;
}

void setWndRectInfo(float x, float y, float width, float height)
{
    wndRtInfo.position.x = x;
    wndRtInfo.position.y = y;
    wndRtInfo.size.width = width;
    wndRtInfo.size.height = height;
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
    clip.left = wndRtInfo.position.x;
    clip.top = wndRtInfo.position.y;
    clip.right = wndRtInfo.position.x + wndRtInfo.size.width;
    clip.bottom = wndRtInfo.position.y + wndRtInfo.size.height;

    ClipCursor(&clip);

    if (x < 0.f + MOUSE_WRAPPING_MARGIN)
        SetCursorPos(wndRtInfo.position.x + (wndRtInfo.size.width - MOUSE_WRAPPING_MARGIN),
            wndRtInfo.position.y + y);
    else if (x > wndRtInfo.size.width - MOUSE_WRAPPING_MARGIN)
        SetCursorPos(wndRtInfo.position.x + MOUSE_WRAPPING_MARGIN,
            wndRtInfo.position.y + y);

    if (y < 0.f + MOUSE_WRAPPING_MARGIN)
        SetCursorPos(wndRtInfo.position.x + x,
            wndRtInfo.position.y + (wndRtInfo.size.height - MOUSE_WRAPPING_MARGIN));
    else if (y > wndRtInfo.size.height - MOUSE_WRAPPING_MARGIN)
        SetCursorPos(wndRtInfo.position.x + x,
            wndRtInfo.position.y + MOUSE_WRAPPING_MARGIN);
}

void wrapCursor(bool wrap)
{
    wrapping = wrap;
    if (!wrap) ClipCursor(nullptr);
    else SetCursorPos(wndRtInfo.position.x + wndRtInfo.size.width / 2,
                      wndRtInfo.position.y + wndRtInfo.size.height / 2);
}

bool isContinueApp()
{
    return run;
}

void shutDownApp()
{
    run = false;
}

