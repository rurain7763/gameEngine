#pragma once

#define DEV_WIDTH		1920.f
#define DEV_HEIGHT		1080.f

#define DEFAULT_CAMERA_POSITION	{ 0.f, 0.f, -5.f }

#include "iType.h"

class iInputManager;
class iTime;
class iAssetReader;
class iGLShader;
class iMatrix;
class iCamera;

void loadGame();
void drawGame();
void endGame();

class iInputManager;
extern iInputManager* inputMgt;

extern iSize* devSize;


