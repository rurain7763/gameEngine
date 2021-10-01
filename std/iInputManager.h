#pragma once

#include "iType.h"
#include "iVector.h"

#define KEY_W				0x1 
#define KEY_S				0x2 
#define KEY_A				0x4 
#define KEY_D				0x8 
#define KEY_ESCAPE			0x10 
#define KEY_P				0x20 
#define KEY_PAGEUP			0x40 
#define KEY_PAGEDOWN		0x80 
#define KEY_1				0x100 
#define KEY_2				0x200 
#define KEY_3				0x400 
#define KEY_4				0x800 

typedef unsigned int iKey;

class iInputManager
{
private:
	static iInputManager* S;
	iInputManager();

public:
	static iInputManager* share();

	void setKeyDown(iKey key);
	void setKeyUp(iKey key);
	void setMousePos(float x, float y);
	void update();

private:
	uint32 keyBuffer;

public:
	uint32 keyOnce;
	uint32 keyDown;
	iVector2f mousePos;
}; 

