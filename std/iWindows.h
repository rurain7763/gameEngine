#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define USE_SOCKET true

#include <winsock2.h>
#include <ws2tcpip.h>
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