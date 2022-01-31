#pragma once

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include "iType.h"

#include "iVector.h"
#include "iStack.h"
#include "iString.h"
#include "iArray.h"
#include "iHashTable.h"

#pragma comment (lib,"Gdiplus.lib")

#define IGRAPHICS_STACK_SIZE 20
#define IGRAPHICS_COLOR		255.f * color.a, \
							255.f * color.r, \
							255.f * color.g, \
							255.f * color.b

class iImage;

using namespace Gdiplus;

class iGraphics
{
public:
	iGraphics(iSize& devSize);
	virtual ~iGraphics();

	void clear();

	void pushStat();
	void popStat();

	iImage* getiImage();

	void drawString(const char* str, iVector2f pos);

private:
	Font* getFont(const char* fontName, int size);

private:
	iStack stat;

	PrivateFontCollection* collection;
	iHashTable fonts;
	iArray fontNames;

	Graphics* graphics;
	Bitmap* bmp;

public:
	iString font;

	uint8 size;
	iColor color;
};

