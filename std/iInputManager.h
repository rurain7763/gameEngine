#pragma once

#include "iType.h"
#include "iVector.h"

#define CHANGE_LANG_BUTTON	VK_F1

#define LANG_MODE_NATIVE	0
#define LANG_MODE_LOCALE	1

#define KEY_w				0x1
#define KEY_s				0x2 
#define KEY_a				0x4 
#define KEY_d				0x8 
#define KEY_ESCAPE			0x10 
#define KEY_p				0x20 
#define KEY_PAGEUP			0x40 
#define KEY_PAGEDOWN		0x80 
#define KEY_1				0x100 
#define KEY_2				0x200 
#define KEY_3				0x400 
#define KEY_4				0x800 
#define KEY_q				0x1000 
#define KEY_e				0x2000 
#define KEY_r				0x4000 
#define KEY_t				0x8000 
#define KEY_y				0x10000 
#define KEY_u				0x20000 
#define KEY_i				0x40000 
#define KEY_o				0x80000 
#define KEY_f				0x100000 
#define KEY_g				0x200000 
#define KEY_h				0x400000 
#define KEY_j				0x800000 
#define KEY_k				0x1000000 
#define KEY_l				0x2000000 
#define KEY_z				0x4000000 
#define KEY_x				0x8000000 
#define KEY_c				0x10000000 
#define KEY_v				0x20000000 
#define KEY_b				0x40000000 
#define KEY_n				0x80000000 
#define KEY_m				0x100000000
#define KEY_SPACE			0x200000000
#define KEY_Q				0x400000000
#define KEY_W				0x800000000
#define KEY_E				0x1000000000
#define KEY_R				0x2000000000
#define KEY_T				0x4000000000
#define KEY_O				0x8000000000
#define KEY_P				0x10000000000
#define KEY_BACK			0x20000000000

typedef uint64 iKey;

class iInputManager
{
private:
	static iInputManager* S;
	iInputManager();

public:
	static iInputManager* share();

	void changeLanguageMode();
	void setKeyDown(iKey key);
	void setKeyUp(iKey key);
	void setMousePos(float x, float y);
	void update();

private:
	uint64 keyBuffer;

public:
	uint8 langMode;
	uint64 keyOnce;
	uint64 keyDown;
	iVector2f mousePos;
}; 

