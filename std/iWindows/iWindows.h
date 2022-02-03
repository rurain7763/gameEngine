#pragma once

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include "iGraphics.h"

#include <windows.h>
#include <Windowsx.h>

#include "iType.h"

#pragma comment(lib, "imm32.lib")

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

wchar_t* multiByteToWideChar(const char* str, ...);
char* wideCharToMultiByte(const wchar_t* str);


