#pragma once

#include <windows.h>
#include <Windowsx.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"
#pragma comment(lib, "pthreadVC2.lib")

#include "iType.h"

void loadGdi();
void endGdi();

iRect getWndRectInfo();

void setWndPosInfo(float x, float y);
void setWndSizeInfo(float width, float height);
void setWndRectInfo(iRect& rt);
void setWndRectInfo(float x, float y, float width, float height);

void showCursor(bool show);

void wrappingCursor(float& mouseX, float& mouseY);
void wrapCursor(bool wrap);

bool isContinueApp();
void shutDownApp();