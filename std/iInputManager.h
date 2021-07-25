#pragma once

#include"iVector.h"

#define KEY_W			1
#define KEY_S			2
#define KEY_A			4
#define KEY_D			8
#define KEY_ESCAPE		16
#define KEY_P			32

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
	int keyBuffer;

public:
	int keyOnce;
	int keyDown;
	iVector2f mousePos;
}; 

