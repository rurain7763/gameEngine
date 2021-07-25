#pragma once

#define DEV_WIDTH		1920.f
#define DEV_HEIGHT		1080.f

void loadGame();
void drawGame();
void endGame();

class iInputManager;
extern iInputManager* inputMgt;


