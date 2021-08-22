#pragma once

#define DEV_WIDTH		1920.f
#define DEV_HEIGHT		1080.f

#include "iType.h"

void loadGame();
void drawGame();
void endGame();

class iInputManager;
extern iInputManager* inputMgt;

extern iSize* devSize;


