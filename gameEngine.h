#pragma once

#define WIN32_LEAN_AND_MEAN
#include "resource.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

#define MOUSE_MARGIN	20.f

void showCursor(bool show);
void wrapCursor(bool wrap);
