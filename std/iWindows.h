#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define USE_SOCKET true

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <windows.h>
#include <Windowsx.h>
#include <objidl.h>
#include <gdiplus.h>

#include "iType.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

void loadGdi();
void endGdi();

void loadWSA();
void endWSA();

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